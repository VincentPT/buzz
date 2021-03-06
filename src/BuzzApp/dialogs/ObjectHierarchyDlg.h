#pragma once
#include "Win32Dlg.h"
#include "..\drawobjs\BuzzDrawObj.h"
#include <list>
#include <functional>
#include <Commctrl.h>
class BuzzWindow;

class ObjectHierarchyDlg : public Win32Dlg {
public:
	typedef std::function<void(BuzzDrawObj* obj, BOOL)> ItemCheckChangedHandler;
protected:
	BuzzDrawObjRef _rootObject;
	HWND		_hTreeView;
	ItemCheckChangedHandler _onCheckChanged;
protected:
	virtual INT_PTR processMessage(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void onInit();
	virtual void onDestroy();
	virtual void onItemSelectionChanged(LPNMTREEVIEW selectionChangeInfo);
	virtual void onItemCheckedChanged(HTREEITEM hItem, BOOL newState);
	void checkAllSubItems(HWND hTreeView, HTREEITEM hItem, BOOL check);
public:
	ObjectHierarchyDlg(HWND hwndParent);
	virtual ~ObjectHierarchyDlg();

	void setObjectRoot(BuzzDrawObjRef object);
	void setItemCheckChangedHandler(ItemCheckChangedHandler&& handler);
};