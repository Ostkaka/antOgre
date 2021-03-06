#ifndef BASERENDERCOMPONENT_HPP
	#define BASERENDERCOMPONENT_HPP

#include <actors/ActorComponent.hpp>
#include <antOgre/IOGRERenderComponent.hpp>

namespace antOgre
{
	/**
	* Base render component for ogre objects
	*/
	class BaseOGRERenderComponent: public antOgre::IOGRERenderComponent
	{
	public:

		BaseOGRERenderComponent();

		/// Inits component from XML data
		virtual bool init(TiXmlElement* pData);

		/// post init
		virtual void postInit(void);

		/// updates the components and it's scene node from changes
		virtual void onChanged(void);

		/// Generates XML description from the component
		virtual TiXmlElement* generateXml(void);

		/// Get the color from the component
		const Ogre::ColourValue& getColor(void) const;

	protected:

		/// Factory method for creating scene node. Implemented for each sub component
		virtual Ogre::SceneNode* createSceneNode(Ogre::SceneManager* mgr) = 0;

		/// Load color values
		Ogre::ColourValue loadColor(TiXmlElement * pColorData);

		//// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) { return true; }
		
		// Should this return a render component?	
		virtual Ogre::SceneNode* getSceneNode(Ogre::SceneManager* mgr);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		Ogre::SceneNode*        m_sceneNode;
		Ogre::ColourValue       m_color;
	};

	ANT_INLINE const  Ogre::ColourValue& BaseOGRERenderComponent::getColor(void) const { return m_color; }

	ANT_DECLARE_POINTER_TYPES(BaseEntityRenderComponent)
	/**
	* Render component that is based on an existing ogre entity in the Ogre resource system
	*/
	class BaseEntityRenderComponent : public BaseOGRERenderComponent
	{
	public:
		/// Default constructor
		BaseEntityRenderComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual Ogre::SceneNode* createSceneNode(Ogre::SceneManager* mgr) ANT_OVERRIDE;

		//// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;
	
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		std::string m_entityName;

	public:
		static const char *g_Name;
	};

	/**
	* Render component responsible for creating a skybox 
	*/
	class SkyRenderComponent : public BaseOGRERenderComponent
	{
	public:
		/// Default constructor
		SkyRenderComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual Ogre::SceneNode* createSceneNode(Ogre::SceneManager* mgr) ANT_OVERRIDE;

		//// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		std::string m_textureName;

	public:
		static const char *g_Name;
	};

	/**
	* Class that handles a light
	*/
	class LightRenderComponent : public BaseOGRERenderComponent
	{
	public:

		struct LightProperties
		{
			float	m_Attenuation[3];  /* Attenuation coefficients */
			float	m_Range;
			float	m_Falloff;
			float	m_Theta;
			float	m_Phi;
		};

		LightRenderComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual Ogre::SceneNode* createSceneNode(Ogre::SceneManager* mgr) ANT_OVERRIDE;

		//// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		std::string     m_textureName;
		LightProperties m_Props;
	public:
		static const char *g_Name;
	};


	/**
	* Render component responsible for creating a skybox
	*/
	class BoxRenderComponent : public BaseOGRERenderComponent
	{
	public:
		/// Default constructor
		BoxRenderComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual Ogre::SceneNode* createSceneNode(Ogre::SceneManager* mgr) ANT_OVERRIDE;

		//// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ant::Vec3 m_halfVector;

	public:
		static const char *g_Name;
	};

	/**
	* Render component responsible for creating a skybox
	*/
	class SphereRenderComponent : public BaseOGRERenderComponent
	{
	public:
		/// Default constructor
		SphereRenderComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual Ogre::SceneNode* createSceneNode(Ogre::SceneManager* mgr) ANT_OVERRIDE;

		//// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ant::Real m_radius;

	public:
		static const char *g_Name;
	};

	/**
	* Render component responsible for creating a skybox
	*/
	class PlaneRenderComponent : public BaseOGRERenderComponent
	{
	public:
		/// Default constructor
		PlaneRenderComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual Ogre::SceneNode* createSceneNode(Ogre::SceneManager* mgr) ANT_OVERRIDE;

		//// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ant::Vec3 m_size;

	public:
		static const char *g_Name;
	};
}

#endif