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

#include "PipeOutdoor.hpp"
#include "PipeOutdoor_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Construction.hpp"
#include "Construction_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Pipe_Outdoor_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PipeOutdoor_Impl::PipeOutdoor_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PipeOutdoor::iddObjectType());
  }

  PipeOutdoor_Impl::PipeOutdoor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PipeOutdoor::iddObjectType());
  }

  PipeOutdoor_Impl::PipeOutdoor_Impl(const PipeOutdoor_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PipeOutdoor_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PipeOutdoor_Impl::iddObjectType() const {
    return PipeOutdoor::iddObjectType();
  }

  unsigned PipeOutdoor_Impl::inletPort()
  {
    return 0;
  }

  unsigned PipeOutdoor_Impl::outletPort()
  {
    return 0;
  }

  boost::optional<Construction> PipeOutdoor_Impl::construction() const {
    return getObject<ModelObject>().getModelObjectTarget<Construction>(OS_Pipe_OutdoorFields::Construction);
  }

  boost::optional<Connection> PipeOutdoor_Impl::fluidInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Pipe_OutdoorFields::FluidInletNode);
  }

  boost::optional<Connection> PipeOutdoor_Impl::fluidOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Pipe_OutdoorFields::FluidOutletNode);
  }

  boost::optional<Node> PipeOutdoor_Impl::ambientTemperatureOutdoorAirNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNode);
  }

  double PipeOutdoor_Impl::pipeInsideDiameter() const {
    boost::optional<double> value = getDouble(OS_Pipe_OutdoorFields::PipeInsideDiameter,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PipeOutdoor_Impl::pipeLength() const {
    boost::optional<double> value = getDouble(OS_Pipe_OutdoorFields::PipeLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PipeOutdoor_Impl::setConstruction(const boost::optional<Construction>& construction) {
    bool result(false);
    if (construction) {
      result = setPointer(OS_Pipe_OutdoorFields::Construction, construction.get().handle());
    }
    else {
      resetConstruction();
      result = true;
    }
    return result;
  }

  void PipeOutdoor_Impl::resetConstruction() {
    bool result = setString(OS_Pipe_OutdoorFields::Construction, "");
    OS_ASSERT(result);
  }

  bool PipeOutdoor_Impl::setFluidInletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Pipe_OutdoorFields::FluidInletNode, connection.get().handle());
    }
    else {
      resetFluidInletNode();
      result = true;
    }
    return result;
  }

  void PipeOutdoor_Impl::resetFluidInletNode() {
    bool result = setString(OS_Pipe_OutdoorFields::FluidInletNode, "");
    OS_ASSERT(result);
  }

  bool PipeOutdoor_Impl::setFluidOutletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Pipe_OutdoorFields::FluidOutletNode, connection.get().handle());
    }
    else {
      resetFluidOutletNode();
      result = true;
    }
    return result;
  }

  void PipeOutdoor_Impl::resetFluidOutletNode() {
    bool result = setString(OS_Pipe_OutdoorFields::FluidOutletNode, "");
    OS_ASSERT(result);
  }

  bool PipeOutdoor_Impl::setAmbientTemperatureOutdoorAirNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNode, node.get().handle());
    }
    else {
      resetAmbientTemperatureOutdoorAirNode();
      result = true;
    }
    return result;
  }

  void PipeOutdoor_Impl::resetAmbientTemperatureOutdoorAirNode() {
    bool result = setString(OS_Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNode, "");
    OS_ASSERT(result);
  }

  bool PipeOutdoor_Impl::setPipeInsideDiameter(double pipeInsideDiameter) {
    bool result = setDouble(OS_Pipe_OutdoorFields::PipeInsideDiameter, pipeInsideDiameter);
    return result;
  }

  bool PipeOutdoor_Impl::setPipeLength(double pipeLength) {
    bool result = setDouble(OS_Pipe_OutdoorFields::PipeLength, pipeLength);
    return result;
  }

} // detail

PipeOutdoor::PipeOutdoor(const Model& model)
  : StraightComponent(PipeOutdoor::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PipeOutdoor_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setPipeInsideDiameter();
  OS_ASSERT(ok);
  // ok = setPipeLength();
  OS_ASSERT(ok);
}

IddObjectType PipeOutdoor::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Pipe_Outdoor);
}

boost::optional<Construction> PipeOutdoor::construction() const {
  return getImpl<detail::PipeOutdoor_Impl>()->construction();
}

boost::optional<Connection> PipeOutdoor::fluidInletNode() const {
  return getImpl<detail::PipeOutdoor_Impl>()->fluidInletNode();
}

boost::optional<Connection> PipeOutdoor::fluidOutletNode() const {
  return getImpl<detail::PipeOutdoor_Impl>()->fluidOutletNode();
}

boost::optional<Node> PipeOutdoor::ambientTemperatureOutdoorAirNode() const {
  return getImpl<detail::PipeOutdoor_Impl>()->ambientTemperatureOutdoorAirNode();
}

double PipeOutdoor::pipeInsideDiameter() const {
  return getImpl<detail::PipeOutdoor_Impl>()->pipeInsideDiameter();
}

double PipeOutdoor::pipeLength() const {
  return getImpl<detail::PipeOutdoor_Impl>()->pipeLength();
}

bool PipeOutdoor::setConstruction(const Construction& construction) {
  return getImpl<detail::PipeOutdoor_Impl>()->setConstruction(construction);
}

void PipeOutdoor::resetConstruction() {
  getImpl<detail::PipeOutdoor_Impl>()->resetConstruction();
}

bool PipeOutdoor::setFluidInletNode(const Connection& connection) {
  return getImpl<detail::PipeOutdoor_Impl>()->setFluidInletNode(connection);
}

void PipeOutdoor::resetFluidInletNode() {
  getImpl<detail::PipeOutdoor_Impl>()->resetFluidInletNode();
}

bool PipeOutdoor::setFluidOutletNode(const Connection& connection) {
  return getImpl<detail::PipeOutdoor_Impl>()->setFluidOutletNode(connection);
}

void PipeOutdoor::resetFluidOutletNode() {
  getImpl<detail::PipeOutdoor_Impl>()->resetFluidOutletNode();
}

bool PipeOutdoor::setAmbientTemperatureOutdoorAirNode(const Node& node) {
  return getImpl<detail::PipeOutdoor_Impl>()->setAmbientTemperatureOutdoorAirNode(node);
}

void PipeOutdoor::resetAmbientTemperatureOutdoorAirNode() {
  getImpl<detail::PipeOutdoor_Impl>()->resetAmbientTemperatureOutdoorAirNode();
}

bool PipeOutdoor::setPipeInsideDiameter(double pipeInsideDiameter) {
  return getImpl<detail::PipeOutdoor_Impl>()->setPipeInsideDiameter(pipeInsideDiameter);
}

bool PipeOutdoor::setPipeLength(double pipeLength) {
  return getImpl<detail::PipeOutdoor_Impl>()->setPipeLength(pipeLength);
}

/// @cond
PipeOutdoor::PipeOutdoor(std::shared_ptr<detail::PipeOutdoor_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

