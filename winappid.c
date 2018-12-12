// winappid, adapted by Thomas Wolff
// and ported to cygwin C from a Windows C++ program.
// link options -L/usr/lib/w32api -lole32 -luuid
//
// Win7AppId
// Author: David Roe (didroe)
// The code's a bit rough and ready but it does the job
//
// Compile with VS 2010 express using command:
//     cl /EHsc /D _UNICODE Win7AppId.cpp /link ole32.lib

#include <windows.h>
#include <propkey.h>
#include <shlobj.h>

#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <sys/cygwin.h>

typedef void * * voidrefref;

void die(char * message) {
	printf("Error: %s\n", message);
	exit(1);
}

void doOrDie(HRESULT hr, char * message) {
	if (!SUCCEEDED(hr)) {
		die(message);
	}
}

int main(int argc, char * argv[])
{
	setlocale (LC_CTYPE, "");

	if (argc <= 1) {
		char * bn = strrchr(argv[0], '/');
		if (bn)
		  bn++;
		else
		  bn = argv[0];
		printf("Usage: %s SHORTCUT [APPID]\n", bn);
		printf("\n");
		printf("Display the Windows AppID of a shortcut or set an AppID\n");
		exit(0);
	}

	doOrDie(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED),
		"Failed to initialise COM");

	IShellLink * link;
	doOrDie(CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
				&IID_IShellLink, (voidrefref) &link),
		"Failed to create ShellLink object");

	IPersistFile * file;
	doOrDie(
		link->lpVtbl->QueryInterface(link, &IID_IPersistFile, 
			(voidrefref) &file),
		"Failed to obtain PersistFile interface");

	char * fn = cygwin_create_path(CCP_POSIX_TO_WIN_W, argv[1]);
	if (!fn)
	  die("Path conversion failed");

	if (argc > 2) {
		doOrDie(file->lpVtbl->Load(file, (LPCOLESTR)fn, (DWORD)STGM_READWRITE),
			"Failed to load shortcut file");
	} else {
		doOrDie(file->lpVtbl->Load(file, (LPCOLESTR)fn, STGM_READ | STGM_SHARE_DENY_NONE),
			"Failed to load shortcut file");
	}

	IPropertyStore * store;
	doOrDie(
		link->lpVtbl->QueryInterface(link, &IID_IPropertyStore, 
			(voidrefref) &store),
		"Failed to obtain PropertyStore interface");

	PROPVARIANT pv;
	doOrDie(store->lpVtbl->GetValue(store, &PKEY_AppUserModel_ID, &pv),
		"Failed to retrieve AppId");

	if (pv.vt != VT_EMPTY) {
		if (pv.vt != VT_LPWSTR) {
			printf("Type: %d\n", pv.vt);
			die("Unexpected property value type");
		}

		printf("Current AppId: %ls\n", pv.pwszVal);
	} else {
		printf("No current AppId\n");
	}
	PropVariantClear(&pv);

	if (argc > 2) {
		wchar_t appid[MAX_PATH];
		mbstowcs(appid, argv[2], MAX_PATH);

		printf("New AppId: %ls\n", appid);

		pv.vt = VT_LPWSTR;
		pv.pwszVal = appid;

		doOrDie(store->lpVtbl->SetValue(store, &PKEY_AppUserModel_ID, &pv),
			"Failed to set AppId");

		// Not sure if we need to do this
		pv.pwszVal = NULL;
		PropVariantClear(&pv);

		doOrDie(store->lpVtbl->Commit(store),
			"Failed to commit AppId property");

		doOrDie(file->lpVtbl->Save(file, NULL, TRUE),
			"Failed to save shortcut");
	}

	store->lpVtbl->Release(store);
	file->lpVtbl->Release(file);
	link->lpVtbl->Release(link);

	return 0;
}
