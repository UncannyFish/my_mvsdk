#include "g_local.h"

char propHuntModels[PROPHUNT_MAX_MODELS][MAX_QPATH];
float propHuntModelsBounds[PROPHUNT_MAX_MODELS][2][3];
int propHuntModelsSize = 0;
qboolean propHuntEnabled = qfalse;
qboolean propHuntCountdown = qfalse;
int propHuntRoundStartTime = 0;
int propHuntLastHunter = 0;

void Cmd_PropNext_f(gentity_t *playerEntity)
{
	int model;

	if (trap_Argc() != 1)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Usage: propNext\n");
		return;
	}

	model = playerEntity->client->propHunt.selectedModel + 1;
	PropSelect(playerEntity, model, qfalse);
}

void Cmd_PropPrevious_f(gentity_t *playerEntity)
{
	int model;

	if (trap_Argc() != 1)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Usage: propPrevious\n");
		return;
	}

	model = playerEntity->client->propHunt.selectedModel - 1;
	PropSelect(playerEntity, model, qfalse);
}

void Cmd_PropLast_f(gentity_t *playerEntity)
{
	int model;

	if (trap_Argc() != 1)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Usage: propLast\n");
		return;
	}

	model = playerEntity->client->propHunt.lastPlacedModel;
	PropSelect(playerEntity, model, qfalse);
}

void Cmd_PropSelect_f(gentity_t *playerEntity)
{
	int model;
	char arg[MAX_TOKEN_CHARS];

	if (trap_Argc() != 2)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Usage: propSelect <model ID>\n");
		return;
	}

	trap_Argv(1, arg, sizeof(arg));
	model = atoi(arg);
	PropSelect(playerEntity, model, qtrue);
}

void Cmd_PropDeselect_f(gentity_t *playerEntity)
{
	if (trap_Argc() != 1)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Usage: propDeselect\n");
		return;
	}

	PropDeselect(playerEntity);
}

void Cmd_PropPlace_f(gentity_t *playerEntity)
{
	if (trap_Argc() != 1)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Usage: propPlace\n");
		return;
	}

	PropPlace(playerEntity);
}

void Cmd_PropHelp_f(gentity_t *playerEntity)
{
	if (trap_Argc() != 1)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Usage: propHelp\n");
		return;
	}

	G_ClientPrint(
		playerEntity->client->ps.clientNum,
		"client commands\n"
		"propNext - attach next prop to player from model list\n"
		"propPrevious - attach previous prop to player from model list\n"
		"propLast - attach last placed prop to player from model list\n"
		"propSelect X - attach prop X to player from model list\n"
		"propDeselect - remove prop attachments from player\n"
		"propPlace - place attached prop\n"
		"propHelp - print PropHunt help\n\n"

		"press +use on placed prop to hide in it\n"
		"press +use to unhide\n\n"

		"destroy prop to remove it from placed model\n\n"

		"server commands\n"
		"propAdd X - add X to model list\n"
		"propDelete X - delete X from model list\n"
	);
}

void Svcmd_PropAdd_f(void)
{
	char arg[MAX_TOKEN_CHARS];
	int argc;
	int i;
	argc = trap_Argc();
	if (argc < 2 || argc > 8)
	{
		G_Printf(
			"Usage: propAdd <model> [mins x y z] [maxs x y z]\n"
			"Example: propAdd \"models/map_objects/bespin/jk2logo.md3\" -8 -8 0 8 8 24\n"
		);
		return;
	}
	if (!propHuntEnabled)
	{
		G_Printf("PropHunt disabled\n");
		return;
	}
	trap_Argv(1, arg, sizeof(arg));
	if (propHuntModelsSize >= PROPHUNT_MAX_MODELS)
	{
		G_Printf("Can't add more than %d models\n", PROPHUNT_MAX_MODELS);
		return;
	}
	Q_strncpyz(propHuntModels[propHuntModelsSize], arg, MAX_QPATH);
	VectorSet(propHuntModelsBounds[propHuntModelsSize][PROPHUNT_BOUNDS_MINS], -8.0f, -8.0f, 0.0f);
	VectorSet(propHuntModelsBounds[propHuntModelsSize][PROPHUNT_BOUNDS_MAXS], 8.0f, 8.0f, 24.0f);

	for (i = 2; i <= 4 && i < argc; i++)
	{
		trap_Argv(i, arg, sizeof(arg));
		propHuntModelsBounds[propHuntModelsSize][PROPHUNT_BOUNDS_MINS][i - 2] = atof(arg);
	}

	for (i = 5; i <= 7 && i < argc; i++)
	{
		trap_Argv(i, arg, sizeof(arg));
		propHuntModelsBounds[propHuntModelsSize][PROPHUNT_BOUNDS_MAXS][i - 5] = atof(arg);
	}

	G_ModelIndex(propHuntModels[propHuntModelsSize]);
	propHuntModelsSize++;
}

void Svcmd_PropDelete_f(void)
{
	char prop[MAX_TOKEN_CHARS];
	int i;
	int foundModel = -1;
	if (trap_Argc() != 2)
	{
		G_Printf("Usage: propDelete <model>\n");
		return;
	}
	if (!propHuntEnabled)
	{
		G_Printf("PropHunt disabled\n");
		return;
	}
	trap_Argv(1, prop, sizeof(prop));
	if (propHuntModelsSize == 0)
	{
		G_Printf("No models added\n");
		return;
	}
	for (i = 0; i < propHuntModelsSize; i++)
	{
		if (Q_stricmp(propHuntModels[i], prop) == 0)
		{
			foundModel = i;
			break;
		}
	}
	if (foundModel == -1)
	{
		G_Printf("Can't find model \"%s\"\n", prop);
		return;
	}
	else if (foundModel == (propHuntModelsSize - 1))
	{
		propHuntModels[foundModel][0] = '\0';
		propHuntModelsSize--;
	}
	else
	{
		Q_strncpyz(propHuntModels[foundModel], propHuntModels[propHuntModelsSize - 1], MAX_QPATH);
		propHuntModels[propHuntModelsSize - 1][0] = '\0';
		propHuntModelsSize--;
	}
}

void PropTouch(gentity_t *ent, gentity_t *other, trace_t *trace)
{
	return;
}

void PropUse(gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if (activator->r.svFlags & SVF_NOCLIENT)
	{
		activator->r.svFlags &= ~SVF_NOCLIENT;
		activator->client->ps.pm_type = PM_NORMAL;
		VectorCopy(activator->client->propHunt.originBeforeHiding, activator->client->ps.origin);
		activator->client->propHunt.isHidingInModel = qfalse;
		activator->client->propHunt.hidingModelEntityNumber = 0;
		G_ClientPrint(activator->client->ps.clientNum, "You are visible\n");
	}
	else
	{
		activator->r.svFlags |= SVF_NOCLIENT;
		activator->client->ps.pm_type = PM_FREEZE;
		VectorCopy(activator->client->ps.origin, activator->client->propHunt.originBeforeHiding);
		VectorCopy(self->r.currentOrigin, activator->client->ps.origin);
		activator->client->ps.origin[2] += 20;
		activator->client->propHunt.isHidingInModel = qtrue;
		activator->client->propHunt.hidingModelEntityNumber = self->s.number;
		G_ClientPrint(activator->client->ps.clientNum, "You are hidden\n");
	}
}

void PropThink(gentity_t *ent)
{
	vec3_t fwd, fwdorg;
	vec3_t yawonly;
	int i;
	if (ent->parent->client->propHunt.isModelAttached && ent->parent->client->propHunt.attachedModelEntityNumber == ent->s.number)
	{
		yawonly[ROLL] = 0;
		yawonly[PITCH] = 0;
		yawonly[YAW] = ent->parent->client->ps.viewangles[YAW];
		AngleVectors(yawonly, fwd, NULL, NULL);
		for (i = 0; i < 3; i++)
		{
			fwdorg[i] = ent->parent->client->ps.origin[i] + fwd[i] * 64;
		}
		G_SetOrigin(ent, fwdorg);
	}
	ent->nextthink = level.time + 100;
}

void PropDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
{
	if (self->parent->client->propHunt.isModelAttached && self->parent->client->propHunt.attachedModelEntityNumber == self->s.number)
	{

	}
	else
	{
		self->parent->client->propHunt.placedModelsCount -= 1;
	}
	if (self->parent->client->propHunt.isHidingInModel && self->parent->client->propHunt.hidingModelEntityNumber == self->s.number)
	{
		self->parent->r.svFlags &= ~SVF_NOCLIENT;
		self->parent->client->ps.pm_type = PM_NORMAL;
		VectorCopy(self->parent->client->propHunt.originBeforeHiding, self->parent->client->ps.origin);
		self->parent->client->propHunt.isHidingInModel = qfalse;
		self->parent->client->propHunt.hidingModelEntityNumber = 0;
		G_ClientPrint(self->parent->client->ps.clientNum, "You are visible\n");
	}
	self->parent->client->propHunt.isModelAttached = qfalse;
	self->parent->client->propHunt.attachedModelEntityNumber = 0;
	G_FreeEntity(self);
}

qboolean PropSpawnable(gentity_t *playerEntity)
{
	vec3_t fwd, fwdorg;
	vec3_t yawonly;
	vec3_t mins, maxs;
	vec3_t trtest;
	trace_t tr;
	int i;
	int model;
	//if (playerEntity->client->ps.fd.sentryDeployed)
	if (0)
	{
		G_AddEvent(&g_entities[playerEntity->client->ps.clientNum], EV_ITEMUSEFAIL, SENTRY_ALREADYPLACED);
		return qfalse;
	}
	yawonly[ROLL] = 0;
	yawonly[PITCH] = 0;
	yawonly[YAW] = playerEntity->client->ps.viewangles[YAW];
	model = playerEntity->client->propHunt.selectedModel;
	VectorCopy(propHuntModelsBounds[model][PROPHUNT_BOUNDS_MINS], mins);
	VectorCopy(propHuntModelsBounds[model][PROPHUNT_BOUNDS_MAXS], maxs);
	AngleVectors(yawonly, fwd, NULL, NULL);
	for (i = 0; i < 3; i++)
	{
		fwdorg[i] = playerEntity->client->ps.origin[i] + fwd[i] * 64;
		trtest[i] = fwdorg[i] + fwd[i] * 16;
	}
	trap_Trace(&tr, playerEntity->client->ps.origin, mins, maxs, trtest, playerEntity->client->ps.clientNum, MASK_PLAYERSOLID);
	if ((tr.fraction != 1 && tr.entityNum != playerEntity->client->ps.clientNum) || tr.startsolid || tr.allsolid)
	{
		G_AddEvent(&g_entities[playerEntity->client->ps.clientNum], EV_ITEMUSEFAIL, SENTRY_NOROOM);
		return qfalse;
	}
	return qtrue;
}

void PropSpawn(gentity_t *playerEntity)
{
	vec3_t fwd, fwdorg;
	vec3_t yawonly;
	vec3_t mins, maxs;
	gentity_t *prop;
	const char *propName;
	const char *propNameExtension;
	int model;
	int i;
	if (!PropSpawnable(playerEntity))
	{
		return;
	}
	yawonly[ROLL] = 0;
	yawonly[PITCH] = 0;
	yawonly[YAW] = playerEntity->client->ps.viewangles[YAW];
	AngleVectors(yawonly, fwd, NULL, NULL);
	for (i = 0; i < 3; i++)
	{
		fwdorg[i] = playerEntity->client->ps.origin[i] + fwd[i] * 64;
	}
	prop = G_Spawn();
	prop->classname = "prop";
	model = playerEntity->client->propHunt.selectedModel;
	VectorCopy(propHuntModelsBounds[model][PROPHUNT_BOUNDS_MINS], mins);
	VectorCopy(propHuntModelsBounds[model][PROPHUNT_BOUNDS_MAXS], maxs);
	propName = propHuntModels[model];
	prop->s.modelindex = G_ModelIndex((char *)propName);
	propNameExtension = Q_strrchr(propName, '.');
	if (Q_stricmp(propNameExtension, ".glm") == 0)
	{
		prop->s.g2radius = 30.0f;
		prop->s.modelGhoul2 = 1;
	}
	G_SetOrigin(prop, fwdorg);
	prop->parent = playerEntity;
//	prop->r.contents = CONTENTS_SOLID;
//	prop->s.solid = 2;
//	prop->clipmask = MASK_SOLID;
	VectorCopy(mins, prop->r.mins);
	VectorCopy(maxs, prop->r.maxs);
	prop->boltpoint3 = playerEntity->client->ps.clientNum;
	prop->boltpoint2 = playerEntity->client->sess.sessionTeam;
	for (i = 0; i < 3; i++)
	{
		prop->r.absmin[i] = prop->s.pos.trBase[i] + prop->r.mins[i];
		prop->r.absmax[i] = prop->s.pos.trBase[i] + prop->r.maxs[i];
	}
	prop->s.eType = ET_GENERAL;
	prop->s.pos.trType = TR_GRAVITY;
	prop->s.pos.trTime = level.time;
	prop->touch = PropTouch;
	prop->use = PropUse;
	prop->nextthink = level.time;
	prop->boltpoint4 = ENTITYNUM_NONE;
	prop->bolt_Head = 1000;
	prop->bolt_LLeg = level.time;
	prop->noDamageTeam = playerEntity->client->sess.sessionTeam;
	//playerEntity->client->ps.fd.sentryDeployed = qtrue;
	trap_LinkEntity(prop);
	prop->s.owner = playerEntity->client->ps.clientNum;
	prop->s.shouldtarget = qtrue;
	if (g_gametype.integer >= GT_TEAM)
	{
		prop->s.teamowner = playerEntity->client->sess.sessionTeam;
	}
	else
	{
		prop->s.teamowner = 16;
	}
	if (prop->count == 0)
	{
		prop->count = 40;
	}
	prop->s.bolt1 = 1;
	prop->s.bolt2 = ENTITYNUM_NONE;
	prop->damage = 0;
	VectorSet(prop->r.mins, -8, -8, 0);
	VectorSet(prop->r.maxs, 8, 8, 24);
	G_RunObject(prop);
	prop->think = PropThink;
	prop->nextthink = level.time + FRAMETIME;
	if (!prop->health)
	{
		prop->health = 50;
	}
	prop->takedamage = qtrue;
	prop->die = PropDie;
	prop->physicsObject = qtrue;
	prop->r.svFlags |= SVF_PLAYER_USABLE;
	playerEntity->client->propHunt.attachedModelEntityNumber = prop->s.number;
	playerEntity->client->propHunt.isModelAttached = qtrue;
	G_Sound(prop, CHAN_BODY, G_SoundIndex("sound/chars/turret/startup.wav"));
	G_AddEvent(playerEntity, EV_USE_ITEM0 + HI_SENTRY_GUN, 0);
}

qboolean PropSelect(gentity_t *playerEntity, int model, qboolean checkBounds)
{
	gclient_t *client;
	propHuntClientData_t *propHunt;
	gentity_t *attachedModel;

	client = playerEntity->client;
	propHunt = &client->propHunt;

	if (!propHuntEnabled)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "PropHunt disabled\n");
		return qfalse;
	}

	if (propHunt->placedModelsCount >= g_propHuntMaxPlacedModels.integer)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Can't place more than %d props\n", g_propHuntMaxPlacedModels.integer);
		return qfalse;
	}

	if (propHunt->isHidingInModel)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Can't place while hiding\n");
		return qfalse;
	}

	if (!client->pers.isSurvivor)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Hunter can't place props\n");
		return qfalse;
	}

	propHunt->selectedModel = model;

	if (propHunt->selectedModel < 0)
	{
		if (checkBounds)
		{
			G_ClientPrint(playerEntity->client->ps.clientNum, "ID must be in range from %d to %d\n", 0, propHuntModelsSize - 1);
			return qfalse;
		}
		else
		{
			propHunt->selectedModel = propHuntModelsSize - 1;
		}
	}

	if (propHunt->selectedModel >= propHuntModelsSize)
	{
		if (checkBounds)
		{
			G_ClientPrint(playerEntity->client->ps.clientNum, "ID must be in range from %d to %d\n", 0, propHuntModelsSize - 1);
			return qfalse;
		}
		else
		{
			propHunt->selectedModel = 0;
		}
	}

	if (propHunt->isModelAttached)
	{
		attachedModel = &g_entities[propHunt->attachedModelEntityNumber];
		attachedModel->die(attachedModel, NULL, NULL, 0, 0);
		propHunt->isModelAttached = qfalse;
	}

	PropSpawn(playerEntity);
	return qtrue;
}

qboolean PropDeselect(gentity_t *playerEntity)
{
	gclient_t *client;
	propHuntClientData_t *propHunt;
	gentity_t *attachedModel;

	client = playerEntity->client;
	propHunt = &client->propHunt;

	if (!propHuntEnabled)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "PropHunt disabled\n");
		return qfalse;
	}

	if (!client->pers.isSurvivor)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Hunter can't place props\n");
		return qfalse;
	}

	if (!propHunt->isModelAttached)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "No prop selected\n");
		return qfalse;
	}

	attachedModel = &g_entities[propHunt->attachedModelEntityNumber];
	attachedModel->die(attachedModel, NULL, NULL, 0, 0);
	propHunt->isModelAttached = qfalse;
	propHunt->selectedModel = 0;
	propHunt->attachedModelEntityNumber = 0;
	return qtrue;
}

qboolean PropPlace(gentity_t *playerEntity)
{
	gclient_t *client;
	propHuntClientData_t *propHunt;
	gentity_t *attachedModel;

	client = playerEntity->client;
	propHunt = &client->propHunt;

	if (!propHuntEnabled)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "PropHunt disabled\n");
		return qfalse;
	}

	if (propHunt->placedModelsCount >= g_propHuntMaxPlacedModels.integer)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Can't place more than %d props\n", g_propHuntMaxPlacedModels.integer);
		return qfalse;
	}

	if (propHunt->isHidingInModel)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Can't place while hiding\n");
		return qfalse;
	}

	if (!client->pers.isSurvivor)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "Hunter can't place props\n");
		return qfalse;
	}

	if (!propHunt->isModelAttached)
	{
		G_ClientPrint(playerEntity->client->ps.clientNum, "No prop selected\n");
		return qfalse;
	}

	G_ClientPrint(playerEntity->client->ps.clientNum, "Placed model %d: \'%s\'\n", propHunt->selectedModel, propHuntModels[propHunt->selectedModel]);

	attachedModel = &g_entities[propHunt->attachedModelEntityNumber];

	propHunt->lastPlacedModel = propHunt->selectedModel;
	propHunt->selectedModel = 0;
	propHunt->placedModelsCount += 1;
	propHunt->attachedModelEntityNumber = 0;
	propHunt->isModelAttached = qfalse;

	attachedModel->r.contents = CONTENTS_SOLID;
	//attachedModel->s.solid = 2;
	attachedModel->clipmask = MASK_SOLID;

	G_RunObject(attachedModel);
	return qtrue;
}

void PropResetInfo(gentity_t *playerEntity)
{
	propHuntClientData_t *ph;
	ph = &playerEntity->client->propHunt;
	ph->selectedModel = 0;
	ph->placedModelsCount = 0;
	ph->attachedModelEntityNumber = 0;
	ph->hidingModelEntityNumber = 0;
	ph->lastPlacedModel = 0;
	VectorSet(ph->originBeforeHiding, 0, 0, 0);
	ph->isModelAttached = qfalse;
	ph->isHidingInModel = qfalse;
	playerEntity->client->pers.isSurvivor = qfalse;
	playerEntity->r.svFlags &= ~SVF_NOCLIENT;
	playerEntity->client->ps.pm_type = PM_NORMAL;
}

void PropHuntInit(void)
{
	if (!g_propHunt.integer)
	{
		propHuntEnabled = qfalse;
		return;
	}
	if (g_propHunt.integer && g_gametype.integer != GT_TEAM)
	{
		G_Warning("PropHunt disabled, set g_gametype to %d\n", GT_TEAM);
		propHuntEnabled = qfalse;
		return;
	}
	propHuntEnabled = qtrue;
	trap_SendConsoleCommand(EXEC_APPEND, va("exec %s\n", g_propHuntModels.string));
}

void PropHuntShutdown(void)
{
	propHuntEnabled = qfalse;
}

void PropHuntFrame(void)
{
	gentity_t *ent;
	gclient_t *client;
	int i, j;
	int modelCount[MAX_CLIENTS] = { 0 };
	int hunters = 0;
	int survivors = 0;

	for (i = 0; i < MAX_GENTITIES; i++)
	{
		ent = &g_entities[i];

		if (!ent->inuse)
		{
			continue;
		}

		if (Q_stricmp(ent->classname, "prop") != 0)
		{
			continue;
		}

		if (ent->parent == NULL || ent->parent->client == NULL)
		{
			G_Warning("PropHunt: prop owner is NULL\n");
			G_FreeEntity(ent);
			continue;
		}

		client = ent->parent->client;

		if (client->sess.sessionTeam != TEAM_BLUE)
		{
			G_Warning("PropHunt: prop owner is not survivor\n");
			PropResetInfo(ent->parent);
			G_FreeEntity(ent);
			continue;
		}

		if (client->propHunt.placedModelsCount == 0 && !client->propHunt.isModelAttached)
		{
			G_Warning("PropHunt: prop owner have zero placed models\n");
			PropResetInfo(ent->parent);
			G_FreeEntity(ent);
			continue;
		}

		for (j = 0; j < MAX_CLIENTS; j++)
		{
			if (ent->parent != &g_entities[j])
			{
				continue;
			}

			if (!g_entities[j].inuse)
			{
				G_Warning("PropHunt: prop owner is NULL\n");
				G_FreeEntity(ent);
				continue;
			}

			modelCount[j]++;
		}
	}

	for (i = 0; i < MAX_GENTITIES; i++)
	{
		ent = &g_entities[i];

		if (!ent->inuse)
		{
			continue;
		}

		if (Q_stricmp(ent->classname, "prop") != 0)
		{
			continue;
		}

		client = ent->parent->client;

		for (j = 0; j < MAX_CLIENTS; j++)
		{
			if (ent->parent != &g_entities[j])
			{
				continue;
			}

			if (modelCount[j] > g_propHuntMaxPlacedModels.integer)
			{
				G_Warning("PropHunt: prop owner have invalid model count\n");
				PropResetInfo(ent->parent);
				G_FreeEntity(ent);
				continue;
			}

			if (ent->parent->client->propHunt.isModelAttached && modelCount[j] != ent->parent->client->propHunt.placedModelsCount + 1)
			{
				G_Warning("PropHunt: prop owner have invalid model count\n");
				PropResetInfo(ent->parent);
				G_FreeEntity(ent);
				continue;
			}

			if (!ent->parent->client->propHunt.isModelAttached && modelCount[j] != ent->parent->client->propHunt.placedModelsCount)
			{
				G_Warning("PropHunt: prop owner have invalid model count\n");
				PropResetInfo(ent->parent);
				G_FreeEntity(ent);
				continue;
			}
		}
	}

	for (i = 0; i < level.maxclients; i++)
	{
		if (level.clients[i].pers.connected != CON_CONNECTED)
		{
			continue;
		}
		if (level.clients[i].sess.sessionTeam == TEAM_BLUE && !level.clients[i].pers.isSurvivor)
		{
			G_Warning("PropHunt: hunter in blue team\n");
			SetTeam(&g_entities[i], "red");
		}
	}

	hunters = TeamCount(-1, TEAM_RED);
	survivors = TeamCount(-1, TEAM_BLUE);

	if (survivors == 0 && hunters > 1 && !propHuntCountdown)
	{
		G_ClientPrint(-1, "New round will begin in 6 seconds\n");
		propHuntRoundStartTime = level.time + 6000;
		propHuntCountdown = qtrue;
	}

	if (survivors == 0 && hunters <= 1 && propHuntCountdown && propHuntRoundStartTime <= level.time)
	{
		propHuntCountdown = qfalse;
	}

	if (survivors == 0 && hunters > 1 && propHuntCountdown && propHuntRoundStartTime <= level.time)
	{
		int newHunter = 0;
		for (i = propHuntLastHunter + 1; i < level.maxclients; i++)
		{
			if (level.clients[i].pers.connected != CON_CONNECTED)
			{
				continue;
			}
			if (level.clients[i].sess.sessionTeam != TEAM_RED)
			{
				continue;
			}
			newHunter = i;
			break;
		}
		propHuntLastHunter = newHunter;
		for (i = 0; i < level.maxclients; i++)
		{
			if (level.clients[i].pers.connected != CON_CONNECTED)
			{
				continue;
			}

			if (level.clients[i].sess.sessionTeam != TEAM_RED)
			{
				continue;
			}

			if (i != newHunter)
			{
				level.clients[i].pers.isSurvivor = qtrue;
				SetTeam(&g_entities[i], "blue");
				level.clients[i].pers.isSurvivor = qtrue;
			}
		}
		propHuntCountdown = qfalse;
	}
}

void QDECL G_ClientPrint(int clientNumber, const char *fmt, ...)
{
	va_list argptr;
	char text[1024] = "print \"";

	va_start(argptr, fmt);
	Q_vsnprintf(&text[7], sizeof(text) - 7 - 1, fmt, argptr);
	va_end(argptr);

	Q_strcat(text, sizeof(text), "\"");
	trap_SendServerCommand(clientNumber, text);
}

void QDECL G_Warning(const char *fmt, ...)
{
	va_list argptr;
	char text[1024] = S_COLOR_YELLOW "WARNING: ";

	va_start(argptr, fmt);
	Q_vsnprintf(&text[11], sizeof(text) - 11, fmt, argptr);
	va_end(argptr);

	trap_Printf(text);
}
