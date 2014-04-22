#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:

#define NOMINMAX
#include <windows.h>
#include <windowsx.h>

#include <crtdbg.h>

#define _VS2005_
#if _MSC_VER < 1400
#undef _VS2005_
#endif

// [rez] I am saddened that Visual Studio 2010 doesn't support the new C++0x final keyword.  It does support 
// the override keyword as well as a special Microsoft-specific sealed keyword.  The override keyword works 
// exactly as the C++0x spec says it does and the sealed keyword acts like the C++0x final keyword.  I toyed 
// around with whether or not I wanted to include this so I will leave the decision up to you.  If you want 
// to use the new C++0x style of override & final, just uncomment this line.  This works on Visual Studio 2008 
// and up for sure.  I'm not sure about earlier versions.
//#define final sealed


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <mmsystem.h>

#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <memory>
#include <queue>
#include <map>

using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;

#define ANT_DECLARE_POINTER_TYPES(type)             \
class type;                                         \
	typedef shared_ptr< type > type ## StrongPtr;           \
	typedef weak_ptr< type > type ## WeakPtr; 

class NonCopyable
{
public:
	NonCopyable() {}; // Default constructor  

private:  
	NonCopyable(const NonCopyable& x);
	NonCopyable& operator=(const NonCopyable& x);
};  

class Singleton 
{
protected:
	Singleton() {}; // Default constructor  
	virtual ~Singleton() {}
	Singleton(const Singleton& x);
	Singleton& operator=(const Singleton& x);
};


// Game Code Complete - Chapter 12, page 446-447
#if defined(_DEBUG)
#	define ANT_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define ANT_NEW new
#endif

#include <tinyxml.h>

// fast delegate stuff
#include <FastDelegate.h>
using fastdelegate::MakeDelegate;

#pragma warning( disable : 4996 ) // 'function' declared deprecated - gets rid of all those 2005 warnings....

// GameCode #includes
#include <ant/debug.hpp>  // this should be the first of the gcc includes since it defines ANT_ASSERT()
//#include <ant/gccUtils/types.hpp>	
#include <ant/GenericObjectFactory.hpp>

//  AppMsg				- Chapter 9, page 248

struct AppMsg
{
	HWND m_hWnd;
	UINT m_uMsg;
	WPARAM m_wParam;
	LPARAM m_lParam;
};

#if !defined(SAFE_DELETE)
#define SAFE_DELETE(x) if(x) delete x; x=nullptr;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=nullptr; 
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(x) if(x) x->Release(); x=nullptr;
#endif

#ifdef UNICODE
#define _tcssprintf wsprintf
#define tcsplitpath _wsplitpath
#else
#define _tcssprintf sprintf
#define tcsplitpath _splitpath
#endif

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

extern INT WINAPI GameCode4(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow);

