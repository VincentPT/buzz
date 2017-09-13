#include "ObjectHierarchyDlg.h"
#include "resources\resource.h"
#include "drawobjs\BuzzContainer.h"
#include "Windowsx.h"
#include "BuzzWindow.h"


INT_PTR ObjectHierarchyDlg::processMessage(HWND hwndDlg, UINT uMsg, WPARAM wParam,  LPARAM lParam) {
	NMHDR* nmhdr;
	switch (uMsg)
	{
	case WM_NOTIFY:
		nmhdr = (NMHDR*)lParam;
		//if (nmhdr->code = TVN_SELCHANGED) {
		//	onItemSelectionChanged( (LPNMTREEVIEW)lParam );
		//}
		//else
		{
			LPNMHDR lpnmh = (LPNMHDR)lParam;
			TVHITTESTINFO ht = { 0 };

			if (lpnmh->code == NM_CLICK)
			{
				DWORD dwpos = GetMessagePos();

				ht.pt.x = GET_X_LPARAM(dwpos);
				ht.pt.y = GET_Y_LPARAM(dwpos);
				MapWindowPoints(HWND_DESKTOP, lpnmh->hwndFrom, &ht.pt, 1);

				TreeView_HitTest(lpnmh->hwndFrom, &ht);

				if (TVHT_ONITEMSTATEICON & ht.flags)
				{
					//onItemCheckedChanged(ht.hItem);					
					HWND hTreeView = GetDlgItem(_hDlg, IDC_OBJECT_HIERARCHY);
					auto checkState = TreeView_GetCheckState(hTreeView, ht.hItem);
					if (checkState == 1) {
						// the item is being checked, and going to be unchecked
						onItemCheckedChanged(ht.hItem, FALSE);
					}
					else if (checkState == 0) {
						// the item is being unchecked, and going to be checked
						onItemCheckedChanged(ht.hItem, TRUE);
					}
				}
			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			return TRUE;
		case IDCANCEL:
			onCancle();
			return TRUE;
		}
	}
	return Win32Dlg::processMessage(hwndDlg, uMsg, wParam, lParam);
}

void ObjectHierarchyDlg::onItemSelectionChanged(LPNMTREEVIEW selectionChangeInfo) {
}

void ObjectHierarchyDlg::checkAllSubItems(HWND hTreeView, HTREEITEM hItem, BOOL check) {
	auto hChild = TreeView_GetChild(hTreeView, hItem);
	while (hChild) {
		TreeView_SetCheckState(hTreeView, hItem, check);
		checkAllSubItems(hTreeView, hItem, check);
		hChild = TreeView_GetNextSibling(hTreeView, hChild);
	}
}

void ObjectHierarchyDlg::onItemCheckedChanged(HTREEITEM hItem, BOOL newState) {
	HWND hTreeView = GetDlgItem(_hDlg, IDC_OBJECT_HIERARCHY);
	TVITEM item;
	item.mask = TVIF_PARAM | TVIF_HANDLE;
	item.hItem = hItem;

	TreeView_GetItem(hTreeView, &item);

	BuzzDrawObj* attachObj = (BuzzDrawObj*)item.lParam;
	attachObj->setVisible(newState == TRUE);
	_pWindow->needUpdate();
}

void insertItem(HWND hTreeView, HTREEITEM parent, BuzzDrawObj* object) {
	TVINSERTSTRUCT insertStruct;
	insertStruct.hParent = parent;
	insertStruct.hInsertAfter = TVI_LAST;

	auto& name = object->getName();

	auto& item = insertStruct.item;
	item.mask = TVIF_PARAM | TVIF_TEXT | TVIF_STATE;
	item.pszText = (LPSTR)name.c_str();
	item.cchTextMax = (int)(name.size());
	item.lParam = (LPARAM)object;

	bool fCheck = object->isVisible();
	item.stateMask = TVIS_STATEIMAGEMASK;
	item.state = INDEXTOSTATEIMAGEMASK((fCheck) ? 2 : 1);

	HTREEITEM hItem = TreeView_InsertItem(hTreeView, &insertStruct);

	auto pContainer = dynamic_cast<BuzzContainer*>(object);
	if (pContainer != nullptr) {
		auto& children = pContainer->getChildren();
		for (auto &aChildRef : children) {
			insertItem(hTreeView, hItem, aChildRef.get());
		}
	}
}

void ObjectHierarchyDlg::onInit() {
	if (!_rootObject) return;

	HWND hTreeView = GetDlgItem(_hDlg, IDC_OBJECT_HIERARCHY);
	if (hTreeView == NULL) {
		return;
	}
	insertItem(hTreeView, NULL, _rootObject.get());
}

void ObjectHierarchyDlg::onDestroy() {
}

ObjectHierarchyDlg::ObjectHierarchyDlg(HWND hwndParent, BuzzWindow* pWindow) : Win32Dlg(hwndParent, IDD_OBJECT_HIERARCHY), _pWindow(pWindow) {
}

ObjectHierarchyDlg::~ObjectHierarchyDlg() {
}

void ObjectHierarchyDlg::setObjectRoot(BuzzDrawObjRef object) {
	_rootObject = object;
}