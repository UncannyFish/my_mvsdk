// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_snapshot.c -- things that happen on snapshot transition,
// not necessarily every single rendered frame

#include "cg_local.h"

ID_INLINE void CG_LastWeapon(void) { //Called by CG_SetNextSnap, dunno if need to use snap or can use predicted..
	if (!cg.snap)
		return;
	if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR)
		return;
	if (cg.snap->ps.pm_flags & PMF_FOLLOW || cg.snap->ps.pm_type == PM_SPECTATOR)
		return;

	if (!cg.lastWeaponSelect[0])
		cg.lastWeaponSelect[0] = cg.predictedPlayerState.weapon;
	if (!cg.lastWeaponSelect[1])
		cg.lastWeaponSelect[1] = cg.predictedPlayerState.weapon;

	if (cg.lastWeaponSelect[0] != cg.predictedPlayerState.weapon) { //Current does not match selected
		cg.lastWeaponSelect[1] = cg.lastWeaponSelect[0]; //Set last to current
		cg.lastWeaponSelect[0] = cg.predictedPlayerState.weapon; //Set current to selected
	}

}


/*
==================
CG_ResetEntity
==================
*/
static void CG_ResetEntity( centity_t *cent ) {
	// if the previous snapshot this entity was updated in is at least
	// an event window back in time then we can reset the previous event
	if ( cent->snapShotTime < cg.time - EVENT_VALID_MSEC ) {
		cent->previousEvent = 0;
	}

	cent->trailTime = cg.snap->serverTime;

	VectorCopy (cent->currentState.origin, cent->lerpOrigin);
	VectorCopy (cent->currentState.angles, cent->lerpAngles);
	if ( cent->currentState.eType == ET_PLAYER ) {
		CG_ResetPlayerEntity( cent );
	}
}

/*
===============
CG_TransitionEntity

cent->nextState is moved to cent->currentState and events are fired
===============
*/
static void CG_TransitionEntity( centity_t *cent ) {
	cent->currentState = cent->nextState;
	cent->currentValid = qtrue;

	if ( jk2startversion == VERSION_1_02 )
	{ // MVSDK: Version Magic!
		cent->currentState.torsoAnim = MV_MapAnimation104( cent->currentState.torsoAnim );
		cent->currentState.legsAnim = MV_MapAnimation104( cent->currentState.legsAnim );
	}

	if ( cent->currentState.solid == SOLID_BMODEL && cent->currentState.time2 && cgs.mvsdk_svFlags & MVSDK_SVFLAG_SUBMODEL_TIME2 )
	{ // If the entity is a SOLID_BMODEL, has a time2 value != 0 and the server has set the MVSDK_SVFLAGS_SUBMODEL_TIME2 value use the time2 value as modelindex,
	  // because the server wants to use a modelindex that doesn't fit into 8 bits.
		cent->currentState.modelindex = cent->currentState.time2;
	}
	else if ( cent->currentState.solid == SOLID_BMODEL && cent->currentState.modelindex < 0 && cgs.mvsdk_svFlags & MVSDK_SVFLAG_SUBMODEL_WORKAROUND )
	{ // modelindex is transfered as signed 8-bit integer (byte), making submodels > 127 appear as negative numbers on the client.
	  // As a negative modelindex isn't valid for SOLID_BMODEL and leads to errors let's try to compensate the modelindex here if the server tells us to.
		cent->currentState.modelindex += 256;
	}

	// reset if the entity wasn't in the last frame or was teleported
	if ( !cent->interpolate ) {
		CG_ResetEntity( cent );
	}

	// clear the next state.  if will be set by the next CG_SetNextSnap
	cent->interpolate = qfalse;

	// check for events
	CG_CheckEvents( cent );
}


/*
==================
CG_SetInitialSnapshot

This will only happen on the very first snapshot, or
on tourney restarts.  All other times will use 
CG_TransitionSnapshot instead.

FIXME: Also called by map_restart?
==================
*/
void CG_SetInitialSnapshot( snapshot_t *snap ) {
	int				i;
	centity_t		*cent;
	entityState_t	*state;

	cg.snap = snap; 

	if ((cg_entities[snap->ps.clientNum].ghoul2 == NULL) && trap_G2_HaveWeGhoul2Models(cgs.clientinfo[snap->ps.clientNum].ghoul2Model))
	{
		trap_G2API_DuplicateGhoul2Instance(cgs.clientinfo[snap->ps.clientNum].ghoul2Model, &cg_entities[snap->ps.clientNum].ghoul2);
		CG_CopyG2WeaponInstance(FIRST_WEAPON, cg_entities[snap->ps.clientNum].ghoul2);
	}
	BG_PlayerStateToEntityState( &snap->ps, &cg_entities[ snap->ps.clientNum ].currentState, qfalse );

	// sort out solid entities
	CG_BuildSolidList();

	CG_ExecuteNewServerCommands( snap->serverCommandSequence );

	// set our local weapon selection pointer to
	// what the server has indicated the current weapon is
	CG_Respawn();

	for ( i = 0 ; i < cg.snap->numEntities ; i++ ) {
		state = &cg.snap->entities[ i ];
		cent = &cg_entities[ state->number ];

		memcpy(&cent->currentState, state, sizeof(entityState_t));
		//cent->currentState = *state;
		cent->interpolate = qfalse;
		cent->currentValid = qtrue;

		if ( jk2startversion == VERSION_1_02 )
		{ // MVSDK: Version Magic!
			cent->currentState.torsoAnim = MV_MapAnimation104( cent->currentState.torsoAnim );
			cent->currentState.legsAnim = MV_MapAnimation104( cent->currentState.legsAnim );
		}

		if ( cent->currentState.solid == SOLID_BMODEL && cent->currentState.time2 && cgs.mvsdk_svFlags & MVSDK_SVFLAG_SUBMODEL_TIME2 )
		{ // If the entity is a SOLID_BMODEL, has a time2 value != 0 and the server has set the MVSDK_SVFLAGS_SUBMODEL_TIME2 value use the time2 value as modelindex,
		  // because the server wants to use a modelindex that doesn't fit into 8 bits.
			cent->currentState.modelindex = cent->currentState.time2;
		}
		else if ( cent->currentState.solid == SOLID_BMODEL && cent->currentState.modelindex < 0 && cgs.mvsdk_svFlags & MVSDK_SVFLAG_SUBMODEL_WORKAROUND )
		{ // modelindex is transfered as signed 8-bit integer (byte), making submodels > 127 appear as negative numbers on the client.
		  // As a negative modelindex isn't valid for SOLID_BMODEL and leads to errors let's try to compensate the modelindex here if the server tells us to.
			cent->currentState.modelindex += 256;
		}

		CG_ResetEntity( cent );

		// check for events
		CG_CheckEvents( cent );
	}
}


/*
===================
CG_TransitionSnapshot

The transition point from snap to nextSnap has passed
===================
*/
static void CG_TransitionSnapshot( void ) {
	centity_t			*cent;
	snapshot_t			*oldFrame;
	int					i;

	if ( !cg.snap ) {
		CG_Error( "CG_TransitionSnapshot: NULL cg.snap" );
	}
	if ( !cg.nextSnap ) {
		CG_Error( "CG_TransitionSnapshot: NULL cg.nextSnap" );
	}

	// execute any server string commands before transitioning entities
	CG_ExecuteNewServerCommands( cg.nextSnap->serverCommandSequence );

	// if we had a map_restart, set everthing with initial
	if ( !cg.snap ) {
	}

	// clear the currentValid flag for all entities in the existing snapshot
	for ( i = 0 ; i < cg.snap->numEntities ; i++ ) {
		cent = &cg_entities[ cg.snap->entities[ i ].number ];
		cent->currentValid = qfalse;
	}

	// move nextSnap to snap and do the transitions
	oldFrame = cg.snap;
	cg.snap = cg.nextSnap;

	CG_CheckPlayerG2Weapons(&cg.snap->ps, &cg_entities[cg.snap->ps.clientNum]);
	BG_PlayerStateToEntityState( &cg.snap->ps, &cg_entities[ cg.snap->ps.clientNum ].currentState, qfalse );
	cg_entities[ cg.snap->ps.clientNum ].interpolate = qfalse;

	for ( i = 0 ; i < cg.snap->numEntities ; i++ ) {
		cent = &cg_entities[ cg.snap->entities[ i ].number ];
		CG_TransitionEntity( cent );

		// remember time of snapshot this entity was last updated in
		cent->snapShotTime = cg.snap->serverTime;
	}

	cg.nextSnap = NULL;

	// check for playerstate transition events
	if ( oldFrame ) {
		playerState_t	*ops, *ps;

		ops = &oldFrame->ps;
		ps = &cg.snap->ps;
		// teleporting checks are irrespective of prediction
		if ( ( ps->eFlags ^ ops->eFlags ) & EF_TELEPORT_BIT ) {
			cg.thisFrameTeleport = qtrue;	// will be cleared by prediction code
		} else if ( cg_smoothCamera.integer ) {
			cg.thisFrameTeleport = qfalse;  // clear for interpolated player with new camera damping
		}

		// if we are not doing client side movement prediction for any
		// reason, then the client events and view changes will be issued now
		if ( cg.demoPlayback || (cg.snap->ps.pm_flags & PMF_FOLLOW)
			|| cg_nopredict.integer || cg_synchronousClients.integer ) {
			CG_TransitionPlayerState( ps, ops );
		}
	}

}


/*
===================
CG_SetNextSnap

A new snapshot has just been read in from the client system.
===================
*/
static void CG_SetNextSnap( snapshot_t *snap ) {
	int					num;
	entityState_t		*es;
	centity_t			*cent;

	cg.nextSnap = snap;

	CG_CheckPlayerG2Weapons(&cg.snap->ps, &cg_entities[cg.snap->ps.clientNum]);
	BG_PlayerStateToEntityState( &snap->ps, &cg_entities[ snap->ps.clientNum ].nextState, qfalse );
	cg_entities[ cg.snap->ps.clientNum ].interpolate = qtrue;

	// check for extrapolation errors
	for ( num = 0 ; num < snap->numEntities ; num++ ) 
	{
		es = &snap->entities[num];
		cent = &cg_entities[ es->number ];

		memcpy(&cent->nextState, es, sizeof(entityState_t));
		//cent->nextState = *es;

		// if this frame is a teleport, or the entity wasn't in the
		// previous frame, don't interpolate
		if ( !cent->currentValid || ( ( cent->currentState.eFlags ^ es->eFlags ) & EF_TELEPORT_BIT )  ) {
			cent->interpolate = qfalse;
		} else {
			cent->interpolate = qtrue;
		}
	}

	// if the next frame is a teleport for the playerstate, we
	// can't interpolate during demos
	if ( cg.snap && ( ( snap->ps.eFlags ^ cg.snap->ps.eFlags ) & EF_TELEPORT_BIT ) ) {
		cg.nextFrameTeleport = qtrue;
	} else {
		cg.nextFrameTeleport = qfalse;
	}

	// if changing follow mode, don't interpolate
	if ( cg.nextSnap->ps.clientNum != cg.snap->ps.clientNum ) {
		cg.nextFrameTeleport = qtrue;
	}

	// if changing server restarts, don't interpolate
	if ( ( cg.nextSnap->snapFlags ^ cg.snap->snapFlags ) & SNAPFLAG_SERVERCOUNT ) {
		cg.nextFrameTeleport = qtrue;
	}

	// sort out solid entities
	CG_BuildSolidList();

	CG_LastWeapon();
}


/*
========================
CG_ReadNextSnapshot

This is the only place new snapshots are requested
This may increment cgs.processedSnapshotNum multiple
times if the client system fails to return a
valid snapshot.
========================
*/
snapshot_1_02_t	activeSnapshot_1_02; // MVSDK: Only used to receive the new snapshot. We're copying the content over as soon as we have the new snapshot... // Global variable for the qvm compiler...
static snapshot_t *CG_ReadNextSnapshot( void ) {
	qboolean	r;
	snapshot_t	*dest;

	if ( cg.latestSnapshotNum > cgs.processedSnapshotNum + 1000 ) {
		CG_Printf( "WARNING: CG_ReadNextSnapshot: way out of range, %i > %i\n",
			cg.latestSnapshotNum, cgs.processedSnapshotNum );
	}

	while ( cgs.processedSnapshotNum < cg.latestSnapshotNum ) {
		// decide which of the two slots to load it into
		if ( cg.snap == &cg.activeSnapshots[0] ) {
			dest = &cg.activeSnapshots[1];
		} else {
			dest = &cg.activeSnapshots[0];
		}

		// try to read the snapshot from the client system
		cgs.processedSnapshotNum++;

		if ( jk2version == VERSION_1_02 )
		{ // MVSDK: Multiversion magic!
			r = trap_GetSnapshot( cgs.processedSnapshotNum, (snapshot_t*)&activeSnapshot_1_02 );
		}
		else
		{
			r = trap_GetSnapshot( cgs.processedSnapshotNum, dest );
		}

		// FIXME: why would trap_GetSnapshot return a snapshot with the same server time
		if ( cg.snap && r && dest->serverTime == cg.snap->serverTime ) {
			//continue;
		}

		// if it succeeded, return
		if ( r ) {
			if ( jk2version == VERSION_1_02 )
			{ // MVSDK: Multiversion Magic
				static const size_t section1 = (size_t)((char *)&((snapshot_t*)NULL)->ps);
				static const size_t section2 = (size_t)((char *)&((playerState_t*)NULL)->forceRestricted);
				static const size_t section3 = (size_t)((char *)&((playerState_t*)NULL)->saberIndex - (char *)&((playerState_t*)NULL)->forceRestricted);
				static const size_t section4 = (size_t)((char *)(&((snapshot_t*)NULL)->ps) + sizeof(playerState_t) - (char *)&((snapshot_t*)NULL)->ps.saberIndex);
				static const size_t section5 = (size_t)((char *)(&(((snapshot_t*)NULL)[1])) - (char *)&((snapshot_t*)NULL)->numEntities);

				/* Convert the snapshot (mainly because of the playerState) */
				memcpy( dest, &(activeSnapshot_1_02), section1 ); // Copy everything till ps
				memcpy( &(dest->ps), &(activeSnapshot_1_02.ps), section2 ); // Copy everything till ps.forceRestricted
				memset( &(dest->ps.forceRestricted), 0, section3 ); // 0 everything from ps.forceRestricted till ps.saberIndex
				memcpy( &(dest->ps.saberIndex), &(activeSnapshot_1_02.ps.saberIndex), section4 ); // Copy everything starting with ps.saberIndex
				memcpy( &dest->numEntities, &(activeSnapshot_1_02.numEntities), section5 ); // Copy everything after ps
			}
			if ( jk2startversion == VERSION_1_02 )
			{
				/* Convert the animations */
				dest->ps.legsAnim = MV_MapAnimation104(dest->ps.legsAnim);
				dest->ps.legsAnimExecute = MV_MapAnimation104(dest->ps.legsAnimExecute);
				dest->ps.torsoAnim = MV_MapAnimation104(dest->ps.torsoAnim);
				dest->ps.torsoAnimExecute = MV_MapAnimation104(dest->ps.torsoAnimExecute);

				/* Only convert forceDodgeAnim if it really is an animation (forceHandExtend being either HANDEXTEND_TAUNT or HANDEXTEND_DODGE) */
				if ( dest->ps.forceHandExtend == HANDEXTEND_TAUNT || dest->ps.forceHandExtend == HANDEXTEND_DODGE ) dest->ps.forceDodgeAnim = MV_MapAnimation104(dest->ps.forceDodgeAnim);

				/* The following two seem to be unused, but maybe custom cgames make use of them (well, fullAnimExecute seems to not even be set at least once - could probably just leave that one out) */
				dest->ps.fullAnimExecute = MV_MapAnimation104(dest->ps.fullAnimExecute);
				dest->ps.saberAttackSequence = MV_MapAnimation104(dest->ps.saberAttackSequence);

				/* Convert the saberblocks */
				if (dest->ps.saberBlocked > BLOCKED_NONE) {
					dest->ps.saberBlocked++;
				}
			}
			CG_AddLagometerSnapshotInfo( dest );
			return dest;
		}

		// a GetSnapshot will return failure if the snapshot
		// never arrived, or  is so old that its entities
		// have been shoved off the end of the circular
		// buffer in the client system.

		// record as a dropped packet
		CG_AddLagometerSnapshotInfo( NULL );

		// If there are additional snapshots, continue trying to
		// read them.
	}

	// nothing left to read
	return NULL;
}


/*
============
CG_ProcessSnapshots

We are trying to set up a renderable view, so determine
what the simulated time is, and try to get snapshots
both before and after that time if available.

If we don't have a valid cg.snap after exiting this function,
then a 3D game view cannot be rendered.  This should only happen
right after the initial connection.  After cg.snap has been valid
once, it will never turn invalid.

Even if cg.snap is valid, cg.nextSnap may not be, if the snapshot
hasn't arrived yet (it becomes an extrapolating situation instead
of an interpolating one)

============
*/
void CG_ProcessSnapshots( void ) {
	snapshot_t		*snap;
	int				n;

	// see what the latest snapshot the client system has is
	trap_GetCurrentSnapshotNumber( &n, &cg.latestSnapshotTime );
	if ( n != cg.latestSnapshotNum ) {
		if ( n < cg.latestSnapshotNum ) {
			// this should never happen
			CG_Error( "CG_ProcessSnapshots: n < cg.latestSnapshotNum" );
		}
		cg.latestSnapshotNum = n;
	}

	// If we have yet to receive a snapshot, check for it.
	// Once we have gotten the first snapshot, cg.snap will
	// always have valid data for the rest of the game
	while ( !cg.snap ) {
		snap = CG_ReadNextSnapshot();
		if ( !snap ) {
			// we can't continue until we get a snapshot
			return;
		}

		// set our weapon selection to what
		// the playerstate is currently using
		if ( !( snap->snapFlags & SNAPFLAG_NOT_ACTIVE ) ) {
			CG_SetInitialSnapshot( snap );
		}
	}

	// loop until we either have a valid nextSnap with a serverTime
	// greater than cg.time to interpolate towards, or we run
	// out of available snapshots
	do {
		// if we don't have a nextframe, try and read a new one in
		if ( !cg.nextSnap ) {
			snap = CG_ReadNextSnapshot();

			// if we still don't have a nextframe, we will just have to
			// extrapolate
			if ( !snap ) {
				break;
			}

			CG_SetNextSnap( snap );


			// if time went backwards, we have a level restart
			if ( cg.nextSnap->serverTime < cg.snap->serverTime ) {
				CG_Error( "CG_ProcessSnapshots: Server time went backwards" );
			}
		}

		// if our time is < nextFrame's, we have a nice interpolating state
		if ( cg.time >= cg.snap->serverTime && cg.time < cg.nextSnap->serverTime ) {
			break;
		}

		// we have passed the transition from nextFrame to frame
		CG_TransitionSnapshot();
	} while ( 1 );

	// assert our valid conditions upon exiting
	if ( cg.snap == NULL ) {
		CG_Error( "CG_ProcessSnapshots: cg.snap == NULL" );
	}
	if ( cg.time < cg.snap->serverTime ) {
		// this can happen right after a vid_restart
		cg.time = cg.snap->serverTime;
	}
	if ( cg.nextSnap != NULL && cg.nextSnap->serverTime <= cg.time ) {
		CG_Error( "CG_ProcessSnapshots: cg.nextSnap->serverTime <= cg.time" );
	}

	// set cg.frameInterpolation
	if ( cg.nextSnap ) {
		int		delta;

		delta = (cg.nextSnap->serverTime - cg.snap->serverTime);
		if ( delta == 0 ) {
			cg.frameInterpolation = 0;
		} else {
			cg.frameInterpolation = (float)( cg.time - cg.snap->serverTime ) / delta;
		}
	} else {
		cg.frameInterpolation = 0;	// actually, it should never be used, because 
									// no entities should be marked as interpolating
		cg.predictedTimeFrac = 0.0f;
	}
}

