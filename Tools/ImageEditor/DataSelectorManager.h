/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _c34497f7_46b0_401d_aad3_78a03a236b98_
#define _c34497f7_46b0_401d_aad3_78a03a236b98_

#include <string>
#include "sigslot.h"
#include "Data.h"
#include <map>

namespace tools
{
	class DataSelectorManager
	{
	public:
		DataSelectorManager();
		~DataSelectorManager();

		static DataSelectorManager& getInstance();
		static DataSelectorManager* getInstancePtr();

		void initialise();
		void shutdown();

		typedef sigslot::signal2<Data*, bool> EventType;
		EventType* getEvent(const std::string& _dataType);

		// � �������� ��������� ���������� ����
		void changeParent(Data* _parent);

		// � �������� ��������� ������ �������� �������
		void changeParentSelection(Data* _parent, Data* _selectedChild);

	private:
		void clear();

	private:
		static DataSelectorManager* mInstance;
		typedef std::map<std::string, EventType*> MapEvent;
		MapEvent mEvents;
	};
}

#endif