
require("lua\\ActorManager.lua");

-----------------------------------------------------------------------------------------------------------------------
-- Global ActorManager
-----------------------------------------------------------------------------------------------------------------------

ActorManagerSingleton = ActorManager:Create();

-----------------------------------------------------------------------------------------------------------------------
-- Declare global functions for interfacing with the actor manager
-----------------------------------------------------------------------------------------------------------------------

function AddPlayer(scriptObject)
	ActorManagerSingleton:AddPlayer(scriptObject);
end

function RemovePlayer(scriptObject)
	ActorManagerSingleton:RemovePlayer(scriptObject);
end

function AddActor(scriptObject)
	ActorManagerSingleton:AddActor(scriptObject);
end

function RemoveActor(scriptObject)
	ActorManagerSingleton:RemoveActor(scriptObject);
end

function AddCircle(scriptObject)
	ActorManagerSingleton:AddCircle(scriptObject);
end

function RemoveObject(scriptObject)
	ActorManagerSingleton:RemoveObject(scriptObject);
end
