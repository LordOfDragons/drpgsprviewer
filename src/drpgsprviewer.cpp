/* 
 * Digimon RPG Sprite Viewer
 *
 * Copyright (C) 2004, Plüss Roland ( rptd@gmx.net )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// includes
/////////////
#include "drpgsprviewer.h"
#include "foxtoolkit.h"
#include "gui/svWindowMain.h"
#include "common/deCmdLineArgs.h"
#include "common/exceptions.h"


// variables
//////////////



// function definitions
/////////////////////////
// application


// entry point
#ifdef WIN32
#include <shellapi.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	bool doRun = true;
	int i;

	FILE* w32ConH4xFile = freopen("drpgsprviewer.log", "w", stdout);
	if (!w32ConH4xFile) return -1;
	setvbuf(w32ConH4xFile, NULL, _IONBF, 0);

	LPWSTR* tempArgv = nullptr;
	int tempArgvCount = 0;
	char** foxArgs = nullptr;
	int foxArgCount = 0;

	try {
		// convert command line arguments
		tempArgv = CommandLineToArgvW(pCmdLine, &tempArgvCount);
		if (!tempArgv) {
			THROW(dueInvalidParam);
		}

		deCmdLineArgs cmdArgs;
		for (i = 0; i < tempArgvCount; i++) {
			char buffer[256];
			int length = WideCharToMultiByte(CP_UTF8, 0, tempArgv[i], -1, buffer, sizeof(buffer), NULL, NULL);
			if (length < 1) {
				THROW(dueInvalidParam);
			}
			cmdArgs.AddArgument(buffer);
		}

		// FOX requires the arguments in posix form and with program name as first argument
		// fill command line object. easier to work with if more
		// command line arguments arrive later on
		foxArgs = new char* [tempArgvCount + 1];

		const char* const appname = "drpgsprviewer.exe";
		foxArgs[0] = new char[strlen(appname) + 1];
		strcpy(foxArgs[0], appname);
		foxArgCount++;

		int i;
		for (i = 0; i < tempArgvCount; i++) {
			foxArgs[i + 1] = new char[strlen(cmdArgs.GetArgument(i)) + 1];
			strcpy(foxArgs[i + 1], cmdArgs.GetArgument(i));
			foxArgCount++;
		}

		// check command line for start up arguments
		for (i = 0; i < cmdArgs.GetCount(); i++) {
			if (strcmp(cmdArgs.GetArgument(i), "-help") == 0 ||
				strcmp(cmdArgs.GetArgument(i), "--help") == 0) {
				printf("Digimon RPG Sprite Viewer\n"
					"Written By Plüss Roland\n"
					"Release 1.0\n"
					"Contact: roland@rptd.dnsalias.net\n"
					"\n");
				doRun = false;
				break;
			}
		}

		// the rest only happens if we have not something else blocking
		if (doRun) {
			// Make application
			FXApp* app = new FXApp("Digimon RPG Sprite Viewer", "RPTD");
			// Open display
			app->init(foxArgCount, foxArgs);
			// Make window
			svWindowMain* wndMain = new svWindowMain(app, &cmdArgs);
			// Create it
			app->create();
			// Run
			app->run();
		}
	}
	catch (duException e) {
		e.PrintError();
		if (tempArgv) {
			LocalFree(tempArgv);
		}
		if (foxArgs) {
			int i;
			for (i = 0; i < foxArgCount; i++) {
				delete[] foxArgs[i];
			}
			delete[] foxArgs;
		}
		fflush(stdout);
		return -1;
	}

	if (foxArgs) {
		int i;
		for (i = 0; i < foxArgCount; i++) {
			delete[] foxArgs[i];
		}
		delete[] foxArgs;
	}
	fflush(stdout);
	
	return 0;
}

#else
int main( int argc, char **argv ){
	bool doRun = true;
	int i;
	try{
		// fill command line object. easier to work with if more
		// command line arguments arrive later on
		deCmdLineArgs cmdArgs;
		for( i=1; i<argc; i++ ) cmdArgs.AddArgument( argv[ i ] );
		// check command line for start up arguments
		for( i=0; i<cmdArgs.GetCount(); i++ ){
			if( strcmp( cmdArgs.GetArgument( i ), "-help" ) == 0 ||
			strcmp( cmdArgs.GetArgument( i ), "--help" ) == 0 ){
				printf( "Digimon RPG Sprite Viewer\n"
					"Written By Plüss Roland\n"
					"Release 1.0\n"
					"Contact: roland@rptd.dnsalias.net\n"
					"\n" );
				doRun = false;
				break;
			}
		}
		// the rest only happens if we have not something else blocking
		if( doRun ){
			// Make application
			FXApp *app = new FXApp( "Digimon RPG Sprite Viewer", "RPTD" );
			// Open display
			app->init( argc, argv );
			// Make window
			svWindowMain *wndMain = new svWindowMain( app, &cmdArgs );
			// Create it
			app->create();
			// Run
			app->run();
		}
	}catch( duException e ){
		e.PrintError();
		return -1;
	}
	// return
	return 0;
}
#endif
