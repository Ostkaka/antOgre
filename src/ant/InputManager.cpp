#include <ant/InputManager.hpp>
#include <antOgre\RenderManager.hpp>

ant::InputManager* ant::InputManager::g_inputMgr = nullptr;

ant::InputManager::InputManager()
{

}

ant::InputManager::~InputManager()
{
	// Destroy devices

	m_inputMgr->destroyInputObject(m_keyboard);
	m_inputMgr->destroyInputObject(m_mouse);

	// Destroy input system

	OIS::InputManager::destroyInputSystem(m_inputMgr);

	SAFE_DELETE(m_keyboard);
	SAFE_DELETE(m_mouse);
}

void ant::InputManager::startUp()
{
	ANT_ASSERT(g_inputMgr);

	g_inputMgr = new InputManager();

	g_inputMgr->init();
}

void ant::InputManager::shutDown()
{	
	SAFE_DELETE(g_inputMgr);
}

void ant::InputManager::init()
{
	ANT_ASSERT(antOgre::RenderManager::instance()->getRenderWindow());

	size_t hWnd = 0;
	OIS::ParamList paramList;

	antOgre::RenderManager::instance()->getRenderWindow()->getCustomAttribute("WINDOW", &hWnd);

	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	m_inputMgr = OIS::InputManager::createInputSystem(paramList);

	// Create devices

	m_keyboard = static_cast<OIS::Keyboard*>(m_inputMgr->createInputObject(OIS::OISKeyboard, true));
	m_mouse = static_cast<OIS::Mouse*>(m_inputMgr->createInputObject(OIS::OISMouse, true));

	m_objects.push_back(m_keyboard);
	m_objects.push_back(m_mouse);
}

void ant::InputManager::AddListener(OIS::KeyListener* listener)
{
	m_keyboard->setEventCallback(listener);
}

void ant::InputManager::AddListener(OIS::MouseListener* listener)
{
	m_mouse->setEventCallback(listener);
}

void ant::InputManager::captureInput()
{
	for (auto& it : m_objects)
	{
		it->capture();
	}
}
