#pragma once
#include "FFDialog.h"
#include <string>
#include <vector>
#include "../spylib/spylib_interfaces.h"

class ObjectInputerDlg : public FFDialog {
	std::string					_objectAddress;
	std::vector<std::string>    _objectTypes;
	std::vector<std::string>    _sortTypes;
	std::vector<std::string>    _pointArrayTypes;
	int                         _objectTypeIdx = 0;
	int                         _sortTypeIdx = 0;
	int                         _pointArrayTypeIdx = 1;
	ButtonClickEventHandler		_onAddObjectBtnClick;
private:
	void onAddObjectButtonPress();
public:
	ObjectInputerDlg(ci::app::WindowRef window);
	~ObjectInputerDlg();
	int getSelectedTypeIndex() const;
	int getPointArrayTypeIndex() const;
	ObjectInputerDlg& setSelectedTypeIndex(int idx);
	void* getObjectAddress() const;
	ObjectInputerDlg& setObjectAddress(void* address);

	ButtonClickEventHandler& getAddObjectButtonSignal();

public:
	static void* convertToAddress(const std::string& address);
};