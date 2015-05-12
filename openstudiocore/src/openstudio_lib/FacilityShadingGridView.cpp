/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "FacilityShadingGridView.hpp"

#include "OSDropZone.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_ShadingSurface_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Shading Surface Name"
#define SELECTED "All"

// GENERAL
#define TYPE "Type" // read only
#define SHADINGSURFACEGROUPNAME "Shading Surface Group Name" // read only
#define CONSTRUCTIONNAME "Construction Name"
#define TRANSMITTANCESCHEDULENAME "Transmittance Schedule Name"

// FILTERS
#define SHADINGSURFACETYPE "Shading Surface Type"
#define SHADINGSURFACEGROUPNAME "Shading Surface Group Name"
#define ORIENTATION "Orientation"
#define TILT "Tilt"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  FacilityShadingGridView::FacilityShadingGridView(bool isIP, const model::Model & model, QWidget * parent)
    : GridViewSubTab(isIP, model, parent)
  {
    auto modelObjects = subsetCastVector<model::ModelObject>(model.getModelObjects<model::ShadingSurface>());
    std::sort(modelObjects.begin(), modelObjects.end(), ModelObjectNameSorter());

    m_gridController = new FacilityShadingGridController(isIP, "Shading Surface", IddObjectType::OS_ShadingSurface, model, modelObjects);
    m_gridView = new OSGridView(m_gridController, "Shading Surface", "Drop\nShading Surface", false, parent);

    setGridController(m_gridController);
    setGridView(m_gridView);

    // Filters

    QLabel * label = nullptr;

    QVBoxLayout * layout = nullptr;

    auto filterGridLayout = new QGridLayout();
    filterGridLayout->setContentsMargins(7, 4, 0, 8);
    filterGridLayout->setSpacing(5);

    label = new QLabel();
    label->setText("Filters:");
    label->setObjectName("H2");
    filterGridLayout->addWidget(label, filterGridLayout->rowCount(), filterGridLayout->columnCount(), Qt::AlignTop | Qt::AlignLeft);

    // SHADINGSURFACETYPE

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(SHADINGSURFACETYPE);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_typeFilter = new QComboBox();
    m_typeFilter->addItem("Building");
    m_typeFilter->addItem("Site");
    m_typeFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_typeFilter, &QComboBox::currentTextChanged, this, &openstudio::FacilityShadingGridView::typeFilterChanged);

    layout->addWidget(m_typeFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // SHADINGSURFACEGROUPNAME

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(SHADINGSURFACEGROUPNAME);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_nameFilter = new QLineEdit();
    m_nameFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_nameFilter, &QLineEdit::editingFinished, this, &openstudio::FacilityShadingGridView::nameFilterChanged);

    QRegExp nameRegex("^\\S.*");
    auto nameValidator = new QRegExpValidator(nameRegex, this);
    m_nameFilter->setValidator(nameValidator);

    layout->addWidget(m_nameFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // ORIENTATION

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(ORIENTATION);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_orientationFilter = new QComboBox();
    m_orientationFilter->addItem("Horizontal");
    m_orientationFilter->addItem("Vertical");
    m_orientationFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_orientationFilter, &QComboBox::currentTextChanged, this, &openstudio::FacilityShadingGridView::orientationFilterChanged);

    layout->addWidget(m_orientationFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // TILT

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(TILT);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_tiltFilter = new QLineEdit();
    m_tiltFilter->setFixedWidth(0.7 * OSItem::ITEM_WIDTH);
    connect(m_tiltFilter, &QLineEdit::editingFinished, this, &openstudio::FacilityShadingGridView::tiltFilterChanged);

    auto tiltValidator = new QDoubleValidator(this);
    m_tiltFilter->setValidator(tiltValidator);

    label = new QLabel();
    label->setText("rad");
    label->setObjectName("H3");

    auto hLayout = new QHBoxLayout();
    hLayout->addWidget(m_tiltFilter, Qt::AlignTop | Qt::AlignLeft);
    hLayout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);
    hLayout->addStretch();
    layout->addLayout(hLayout, Qt::AlignTop | Qt::AlignLeft);

    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    filterGridLayout->setRowStretch(filterGridLayout->rowCount(), 100);
    filterGridLayout->setColumnStretch(filterGridLayout->columnCount(), 100);

    m_gridView->m_contentLayout->addLayout(filterGridLayout);

    m_gridView->m_contentLayout->addSpacing(7);
  }

  void FacilityShadingGridView::nameFilterChanged()
  {
    filterChanged();
  }

  void FacilityShadingGridView::typeFilterChanged(const QString& text)
  {
    filterChanged();
  }

  void FacilityShadingGridView::orientationFilterChanged(const QString& text)
  {
    filterChanged();
  }

  void FacilityShadingGridView::tiltFilterChanged()
  {
    filterChanged();
  }

  void FacilityShadingGridView::filterChanged()
  {
    auto objectSelector = this->m_gridController->getObjectSelector();
    
    auto name = m_nameFilter->text();

    auto tilt = m_tiltFilter->text().toDouble();

    auto type = m_typeFilter->currentText();

    auto orintation = m_orientationFilter->currentText();

    for (auto obj : objectSelector->m_selectorObjects) {

    }

    this->m_gridView->requestRefreshAll();
  }

  void FacilityShadingGridView::onDropZoneItemClicked(OSItem* item)
  {
  }

  void FacilityShadingGridView::addObject(const IddObjectType& iddObjectType)
  {
    if (IddObjectType::OS_ShadingSurface == iddObjectType.value()) {
      //model::ShadingSurface();
    }
  }

  void FacilityShadingGridView::purgeObjects(const IddObjectType& iddObjectType)
  {
    for (auto mo : this->m_model.getConcreteModelObjects<model::ShadingSurface>()){
      mo.remove();
    }
  }

  FacilityShadingGridController::FacilityShadingGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void FacilityShadingGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      //fields.push_back(TYPE);
      //fields.push_back(SHADINGSURFACEGROUPNAME);
      //fields.push_back(CONSTRUCTIONNAME);
      //fields.push_back(TRANSMITTANCESCHEDULENAME);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void FacilityShadingGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void FacilityShadingGridController::addColumns(const QString &category, std::vector<QString> & fields)
  {
    // always show name and selected columns
    fields.insert(fields.begin(), { NAME, SELECTED });

    m_baseConcepts.clear();

    for (const auto &field : fields) {

      if (field == NAME) {
        addNameLineEditColumn(Heading(QString(NAME), false, false),
          false,
          false,
          CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::name),
          CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::setName)
          );
      }
      else {
        if (field == SELECTED) {
          auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
          checkbox->setToolTip("Check to select all rows");
          connect(checkbox.data(), &QCheckBox::stateChanged, this, &FacilityShadingGridController::selectAllStateChanged);

          addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
        }
        else if (field == TYPE) { // COMBO

        }
        else if (field == SHADINGSURFACEGROUPNAME) { // LINE EDIT

        }
        else if (field == CONSTRUCTIONNAME) { // DROP ZONE

        }
        else if (field == TRANSMITTANCESCHEDULENAME) { // DROP ZONE
          //addDropZoneColumn(Heading(QString(TRANSMITTANCESCHEDULENAME)),
          //  CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::transmittanceSchedule),
          //  CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::setTransmittanceSchedule),
          //  boost::optional<std::function<void(model::ShadingSurface*)>>(CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::resetTransmittanceSchedule))
          //  );
        }
        else {
          // unhandled
          OS_ASSERT(false);
        }
      }
    }
  }

  QString FacilityShadingGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void FacilityShadingGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void FacilityShadingGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void FacilityShadingGridController::refreshModelObjects()
  {
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getModelObjects<model::ShadingSurface>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
