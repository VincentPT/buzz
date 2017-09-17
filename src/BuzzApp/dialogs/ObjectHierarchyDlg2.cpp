#include "ObjectHierarchyDlg2.h"
#include "resources\resource.h"
#include "drawobjs\BuzzContainer.h"
#include "Windowsx.h"
#include "BuzzWindow.h"


INT_PTR ObjectHierarchyDlg2::processMessage(HWND hwndDlg, UINT uMsg, WPARAM wParam,  LPARAM lParam) {
	NMHDR* nmhdr;
	LPNMLISTVIEW lpnmlv;
	switch (uMsg)
	{
	case WM_NOTIFY:
		if (_stopNotify) {
			printf_s("_stopNotify\n");
			break;
		}
		nmhdr = (NMHDR*)lParam;
		if (nmhdr->code == LVN_ITEMCHANGED && (((LPNMLISTVIEW)nmhdr)->uChanged & LVIF_STATE)) {
			lpnmlv = (LPNMLISTVIEW)nmhdr;			
			UINT oldCheckState = (LVIS_STATEIMAGEMASK & lpnmlv->uOldState);
			UINT newCheckState = (LVIS_STATEIMAGEMASK & lpnmlv->uNewState);
			if (oldCheckState != newCheckState) {
				onItemCheckedChanged(lpnmlv, (BuzzDrawObj*)lpnmlv->lParam, (newCheckState >> 12) - 1);
			}
		}
		break;
	case WM_SIZE:
		onResize(LOWORD(lParam), HIWORD(lParam));
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

//void ObjectHierarchyDlg2::onItemSelectionChanged(LPNMTREEVIEW selectionChangeInfo) {
//}
//
//void ObjectHierarchyDlg2::checkAllSubItems(HWND hTreeView, HTREEITEM hItem, BOOL check) {
//	auto hChild = TreeView_GetChild(hTreeView, hItem);
//	while (hChild) {
//		TreeView_SetCheckState(hTreeView, hItem, check);
//		checkAllSubItems(hTreeView, hItem, check);
//		hChild = TreeView_GetNextSibling(hTreeView, hChild);
//	}
//}
//
void ObjectHierarchyDlg2::onItemCheckedChanged(LPNMLISTVIEW lpItem, BuzzDrawObj* obj, BOOL newState) {
	if (_onCheckChanged) {		

		_onCheckChanged(obj, newState);
		
		int iItem = lpItem->iItem;
		
		LVITEM lvi;
		lvi.mask = LVIF_PARAM | LVIF_STATE;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFF;
		lvi.iItem = iItem;

		ListView_GetItem(_hListView, &lvi);
		// check if current checked item is also selected...
		if ((lvi.state & LVIS_SELECTED) == 0) return;

		_stopNotify = true;

		// ...then set the same state for other selected items
		// Get the first selected item
		int iPos = ListView_GetNextItem(_hListView, -1, LVNI_SELECTED);
		while (iPos != -1) {
			if (iItem != iPos) {
				ListView_SetCheckState(_hListView, iPos, newState);
				lvi.iItem = iPos;
				ListView_GetItem(_hListView, &lvi);
				_onCheckChanged( ((BuzzDrawObj*)lvi.lParam), newState);
			}
			// Get the next selected item
			iPos = ListView_GetNextItem(_hListView, iPos, LVNI_SELECTED);
		}
		_stopNotify = false;
	}
}

void ObjectHierarchyDlg2::onResize(int w, int h) {
	MoveWindow(_hListView, 0, 0, w, h, TRUE);
}

void insertItem(HWND hTreeView, BuzzDrawObj* object, int indent) {
	auto& name = object->getName();
	int stateIndex = object->isVisible() ? 2 : 1;

	LVITEM lvI;

	// Initialize LVITEM members that are common to all items.
	lvI.pszText = (LPSTR)name.c_str(); // Sends an LVN_GETDISPINFO message.
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM | LVIF_INDENT;
	lvI.stateMask = LVIS_STATEIMAGEMASK;
	lvI.iSubItem = 0;
	lvI.state = INDEXTOSTATEIMAGEMASK(stateIndex);
	lvI.iItem = 0x7FFFFFFF; // insert item at the end of list
	lvI.lParam = (LPARAM)object;
	lvI.iIndent = indent;

	auto iRes = ListView_InsertItem(hTreeView, &lvI);
	// dont know why still need set check staye here
	ListView_SetCheckState(hTreeView, iRes, TRUE);

	auto pContainer = dynamic_cast<BuzzContainer*>(object);
	if (pContainer != nullptr) {
		indent++;
		auto& children = pContainer->getChildren();
		for (auto &aChildRef : children) {
			insertItem(hTreeView, aChildRef.get(), indent);
		}
	}
}

void ObjectHierarchyDlg2::onInit() {
	if (!_rootObject) return;
	_hListView = GetDlgItem(_hDlg, IDC_OBJECT_HIERARCHY2);

	if (_hListView == NULL) {
		return;
	}

	ListView_SetExtendedListViewStyleEx(_hListView, LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES);

	// Create the full-sized icon image lists. 
	auto hLarge = ImageList_Create(GetSystemMetrics(SM_CXICON),
		GetSystemMetrics(SM_CYICON),
		ILC_MASK, 1, 1);

	auto hSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		ILC_MASK, 1, 1);

	//// Add an icon to each image list.  
	//auto hiconItem = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));

	//ImageList_Add(hLarge, hiconItem, NULL);
	//ImageList_Add(hSmall, hiconItem, NULL);

	//DeleteObject(hiconItem);

	// Assign the image lists to the list-view control. 
	ListView_SetImageList(_hListView, hLarge, LVSIL_NORMAL);
	ListView_SetImageList(_hListView, hSmall, LVSIL_SMALL);

	RECT rect;
	GetClientRect(_hListView, &rect);

	LVCOLUMN lvc;
	int iCol;

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = "objects";
	lvc.cx = rect.right;  // Width of column in pixels.

	// Insert the columns into the list view.
	iCol = ListView_InsertColumn(_hListView, 0, &lvc);
	
	insertItem(_hListView, _rootObject.get(), 0);
}

void ObjectHierarchyDlg2::onDestroy() {
}

ObjectHierarchyDlg2::ObjectHierarchyDlg2(HWND hwndParent) : _stopNotify(false), Win32Dlg(hwndParent, IDD_OBJECT_HIERARCHY2) {
}

ObjectHierarchyDlg2::~ObjectHierarchyDlg2() {
}

void ObjectHierarchyDlg2::setObjectRoot(BuzzDrawObjRef object) {
	_rootObject = object;
}

void ObjectHierarchyDlg2::setItemCheckChangedHandler(ItemCheckChangedHandler&& handler) {
	_onCheckChanged = handler;
}