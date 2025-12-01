#pragma once
#ifndef INFORMATION_GUI_H
#define INFORMATION_GUI_H
#include <functional>
#include <unordered_set>
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlabel.h>
#include <QtCore\qobject.h>
#include <QtCore\qdebug.h>
#include "LocMainWidget.h"
#include "Eu4GeoPolData.h"
#include "QtUtils.h"
#include "CultRelContainer.h"
#include "CountryContainer.h"
#include "OwnershipBox.h"
#include "FileManager.h"

class InformationGUI : public QTabWidget {
	Q_OBJECT
public:
	InformationGUI(const Eu4::GeoPolData& refGeoPolCont, const CultRelContainer& refCultRel, const CountryContainer& countryContainer, QWidget* parent = nullptr);
	void loadWidgets();
	void loadProvInfo (Eu4::Province& province) const;
	void initialiseWidgetsInfo();
	void changeCurrentProv(Eu4::Province& prov) const;
	void updateProvinceField(uint16_t Eu4::Province::* memberPtr, const std::string& newData);
signals:
	void signalLoadInfo(Eu4::Province& province) const;
	void signalInitCultRelInfo(const std::unordered_map<std::string, sCulture>& cultureData,
		const std::unordered_map<std::string, sReligion>& religionData) const;
	void signalInitOwnership(const std::unordered_map<std::string, std::string>& data) const;

protected:

private:
	const Eu4::GeoPolData& mRefGeoPolCont;
	const CultRelContainer& mRefCultRelCont;
	const CountryContainer& mRefCountryContainer;
	mutable Eu4::Province* currentProv = nullptr;
	std::unordered_set<DM::iFileDataBase*> activeChanges;
};


#endif // !INFORMATION_GUI_H
