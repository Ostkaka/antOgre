#pragma once
//========================================================================
// Logger.h : 
//
// Part of the GameCode4 Application
//
// GameCode4 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 4th Edition" by Mike McShaffry and David
// "Rez" Graham, published by Charles River Media. 
// ISBN-10: 1133776574 | ISBN-13: 978-1133776574
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the authors a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1133776574/ref=olp_product_details?ie=UTF8&me=&seller=
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code: 
//    http://code.google.com/p/gamecode4/
//
// (c) Copyright 2012 Michael L. McShaffry and David Graham
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser GPL v3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
// http://www.gnu.org/licenses/lgpl-3.0.txt for more details.
//
// You should have received a copy of the GNU Lesser GPL v3
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

//---------------------------------------------------------------------------------------------------------------------
// TYPICAL USAGE:
// 
// 1) Call the ANT_ASSERT() macro just like you would for _ASSERTE()
//		Example -> ANT_ASSERT(ptr != NULL);
// 2) Call the ANT_ERROR() macro to send out an error message
//		Example -> ANT_ERROR("Something Blew Up!");
//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
// LOGGING:
// 
// Calling  GCC_LOG("tag", "Log message") will print:
//  [tag] Log Message
// To enable these logs, you need to either manually call Logger::SetDisplayFlags() or set up a logging.xml file.
// logging.xml should follow this form:
// 
//  <Logger>
//      <Log tag="Actor" debugger="1" file="0"/>
//  </Logger>
// 
// The above chunk will cause all logs sent with the "Actor" tag to be displayed in the debugger.  If you set file 
// to 1 as well, it would log out to a file as well.  Don't check in logging.xml to SVN, it should be a local-only 
// file.
//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------
#include <string>

//---------------------------------------------------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------------------------------------------------

// display flags
const unsigned char LOGFLAG_WRITE_TO_LOG_FILE =		1 << 0;
const unsigned char LOGFLAG_WRITE_TO_DEBUGGER =		1 << 1;


//---------------------------------------------------------------------------------------------------------------------
// This is the public Logger interface.  You must call Init() before any logging can take place and Destroy() when
// you're done to clean it up.  Call SetDisplayFlags() to set the display flags for a particular logging tag.  By 
// default, they are all off.  Although you can, you probably shouldn't call Log() directly.  Use the GCC_LOG() macro 
// instead since it can be stripped out by the compiler for Release builds.
//---------------------------------------------------------------------------------------------------------------------
namespace Logger
{
	// This class is used by the debug macros and shouldn't be accessed externally.
	class ErrorMessenger
	{
		bool m_enabled;

	public:
		ErrorMessenger(void);
		void Show(const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum);
	};

	// construction; must be called at the beginning and end of the program
	void Init(const char* loggingConfigFilename);
	void Destroy(void);
	
	// logging functions
	void Log(const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum);
	void SetDisplayFlags(const std::string& tag, unsigned char flags);
}

