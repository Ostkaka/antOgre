print("Loading script");

sParent = nil;
sChild = nil;

TestScriptProcess = class(ScriptProcess,
{
	count = 0;
	finished = false;
});

function print_table( table )
	for k,v in pairs(table) do
		print(k,v)
	end
end

function TestScriptProcess:onInit()
	print("onInit()");
end

function TestScriptProcess:onUpdate(dt)
	self.count = self.count + 1;
	print("Count: " .. self.count);
	
	if self.count >= 5 then
		self:succeedAndStop();
	end
end

function TestScriptProcess:onSuccess()
	print("success!!");
	self.finished = true;
end

function TestScriptProcess:onFail()
	print("Something failed");
end

function TestProcess()
	sParent = TestScriptProcess:create({frequency = 1});
	sChild = TestScriptProcess:create({frequency = 0.5});
	sParent:attachChild(sChild);
	attachProcess(sParent);
end

-- Declare the event listener
function TestEventListener(eventData)
	print("Inside TestEventListener")
	print("Event recieved in lua: ");
	eventData.m_number = eventData.m_number + 1;
	queueEvent(EventType.EvtData_TestFromLua,eventData);
end

Utils.DumpObject(EventType)

registerEventListener(EventType.EvtData_TestToLua,TestEventListener);

print("Script loaded");
