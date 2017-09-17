#pragma once
#include "Win32Dlg.h"
#include "..\drawobjs\BuzzDrawObj.h"
#include <list>
#include <functional>
#include <Commctrl.h>
class BuzzWindow;

class ObjectHierarchyDlg2 : public Win32Dlg {
public:
	typedef std::function<void(BuzzDrawObj* obj, BOOL)> ItemCheckChangedHandler;
protected:
	BuzzDrawObjRef _rootObject;
	HWND		_hListView;
	ItemCheckChangedHandler _onCheckChanged;
	bool _stopNotify;
protected:
	virtual INT_PTR processMessage(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void onInit();
	virtual void onDestroy();
	virtual void onResize(int w, int h);
	
	//virtual void onItemSelectionChanged(LPNMTREEVIEW selectionChangeInfo);
	virtual void onItemCheckedChanged(LPNMLISTVIEW lpItem, BuzzDrawObj* obj, BOOL newState);
public:
	ObjectHierarchyDlg2(HWND hwndParent);
	virtual ~ObjectHierarchyDlg2();

	void setObjectRoot(BuzzDrawObjRef object);
	void setItemCheckChangedHandler(ItemCheckChangedHandler&& handler);
};