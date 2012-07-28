/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _13d1012b_5951_4566_a159_d955696bffc7_
#define _13d1012b_5951_4566_a159_d955696bffc7_

#include "IControlController.h"
#include "ScopeTextureControl.h"

namespace tools
{

	class FontTextureController :
		public IControlController,
		public sigslot::has_slots<>
	{
	public:
		FontTextureController();
		virtual ~FontTextureController();

		virtual void setTarget(Control* _control);

		virtual void activate();
		virtual void deactivate();

	private:
		void notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection);
		void notifyChangeProperty(PropertyPtr _sender);
		void notifyChangeScope(const std::string& _scope);

		void updateTexture(const std::string& _value);

		void commandGenerateFont(const MyGUI::UString& _commandName, bool& _result);

		void generateFont();
		void generateFontTTFXml(MyGUI::xml::ElementPtr _root, const std::string& _fontName, DataPtr _data);
		void removeFont(const std::string& _fontName);
		void updateResultPropery(const std::string& _fontName, DataPtr _data);

	private:
		ScopeTextureControl* mControl;
		std::string mParentTypeName;
		std::string mScopeName;
		DataPtr mParentData;
		bool mActivated;
	};

}

#endif