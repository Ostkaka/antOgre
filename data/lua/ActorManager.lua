

----------------------------------------------------------------------------------
-- Base processes for the actor manager
----------------------------------------------------------------------------------


----------------------------------------------------------------------------------
-- Manager for the scripted characters in the game
----------------------------------------------------------------------------------
ActorManager = class(nil,
{
	_player = nil,
	_actors = {}
});

function ActorManager:AddPlayer(scriptObject)
	if (self._player ~= nil) then
		print("Adding player when player is active")
	end

	-- add new player
	self._player = scriptObject;

	--Utils.DumpObject(EventType)

	-- Tell the engine we added a player
	queueEvent(EventType.EvtData_Set_Controlled_Actor, self._player:getActorId());
end

function ActorManager:AddActor(scriptObject)
	local actorId = scriptObject:getActorId();
	self._actors[actorId] = scriptObject;
	
	--Utils.DumpObject(scriptObject);
end

function ActorManager:RemovePlayer( scriptObject )
	self._player = nil;
end

function ActorManager:GetPlayer()
	return self._player;
end

function ActorManager:GetActorById(actorId)
	local actor = nil;

	if(self._player and self._player:getActorId() == actorId) then
		actor = self._player;
	end

	-- other actors
	if(not actor) then
		actor = self:GetActor(actorId);
	end

	return actor;
end

function ActorManager:GetActor(actorId)
	return self._actors[actorId];
end

function ActorManager:RemoveActor(actorId)
	self._actors[actorId] = nil
end

function ActorManager:CreateCircle()
	local pos = Vec2:Create(); 
	pos.x = 0; 
	pos.y = 200;
	local rot = 0;
	-- Create random pos around origon
	local sphere = createActor("actors\\circle.xml",pos,rot);
end

function ActorManager:OnPhysicsCollision( scriptObject )
	print("Got collide event")

	local actorA = self:GetActorById(scriptObject.actorA)
	local actorB = self:GetActorById(scriptObject.actorB)

	-- handle collision with spawn events
	if(actorA == nill or actorB == nil) then 
		print("Got bad collision data");
		return;
	end

	--self._handleSpawnerCollision(actorA, actorB);
	local spawnActor = nil;
	local actor = nil;
	if(actorA.actorType == "Player" and actorB.actorType == "spawnActor") then
		actor = actorA;
		spawnActor = actorB;
	end

	if(actorA.actorType == "spawnActor" and actorB.actorType == "Player") then
		actor = actorB;
		spawnActor = actorA;
	end

	if(actor ~= nil and spawnActor ~= nil) then
		local pos = spawnActor:getPos();
		local rot = 0;
		pos.x = pos.x + 10;

		local numSpawn = tonumber(spawnActor.spawnNumber);
		print("numSPaen  "..numSpawn)		
		RemoveActor(spawnActor:getActorId())

		for i=1,numSpawn do			
			createActor("actors\\kamek.xml",pos,rot)
		end

		--queueEvent(EventType.EvtData_Request_Destroy_Actor, spawnActor:getActorId());
	end		
end

function ActorManager:OnPhysicsSeparation( scriptObject )
	print("Got separate event")
end

function ActorManager:OnActorJump( scriptObject )
	-- Send animation changed event for entity
	local animationData = {}
	animationData.actorId = scriptObject.actorId
	animationData.animationId = "jumping"

	queueEvent(EventType.EvtData_ChangeAnimation, animationData);
end

function ActorManager:OnActorStartWalking( scriptObject )
	-- Send animation changed event for entity
	local animationData = {}
	animationData.actorId = scriptObject.actorId
	animationData.animationId = "walking"

	queueEvent(EventType.EvtData_ChangeAnimation, animationData);
end

function ActorManager:OnActorStopWalking( scriptObject )
	-- Send animation changed event for entity
	local animationData = {}
	animationData.actorId = scriptObject.actorId
	animationData.animationId = "standing"

	queueEvent(EventType.EvtData_ChangeAnimation, animationData);
end