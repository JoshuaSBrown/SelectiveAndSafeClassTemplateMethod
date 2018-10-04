#include <type_traits>
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cassert>
#include <memory>

using namespace std;

enum vehicle{
  base,
  truck,
  sedan
};

std::ostream& operator<<(std::ostream& os, vehicle v)
{
  switch(v)
  {
    case base  : os << "base vehicle"; break;
    case truck : os << "truck"       ; break;
    case sedan : os << "sedan"       ; break;
    default    : os.setstate(std::ios_base::failbit);
  }
  return os;
}

class Vehicle {
  public:

    Vehicle() : object_type_(vehicle::base) {};

    virtual ~Vehicle() = default;

    vehicle getVehicleType(){ return object_type_; }

    /// Must be declared for each class is not dependent on object instance
    static vehicle getClassType() { return class_type_; }

  protected:
    /// Only declared once but each class must correctly initialize 
    vehicle object_type_;

  private:
    /// Must be declared for each class is not dependent on object instance
    static const vehicle class_type_;

};

/// Must be declared for each class is not dependent on object instance
const vehicle Vehicle::class_type_ = vehicle::base;

class Truck : public Vehicle {
  public:

    Truck() {
      object_type_ = vehicle::truck;
    };

    /// Must be declared for each class is not dependent on object instance
    static vehicle getClassType() { return class_type_; }

  private:

    /// Must be declared for each class is not dependent on object instance
    static const vehicle class_type_;

};

/// Must be declared for each class is not dependent on object instance
const vehicle Truck::class_type_ = vehicle::truck;

class Sedan : public Vehicle {
  public:

    Sedan() {
      object_type_ = vehicle::sedan;
    };

    /// Must be declared for each class is not dependent on object instance
    static vehicle getClassType() { return class_type_; }

  private:

    /// Must be declared for each class is not dependent on object instance
    static const vehicle class_type_;

};

/// Must be declared for each class is not dependent on object instance
const vehicle Sedan::class_type_ = vehicle::sedan;

class BaseGarage {
  public:
    BaseGarage() {};

    void parkVehicle(shared_ptr<Vehicle> V) { 
      for(auto unallowed_vehicle : unallowed_vehicle_types_){
        if(V->getVehicleType() == unallowed_vehicle){
          stringstream ss;
          ss << "This vehicle " << unallowed_vehicle << " is not allowed in the"
            " garage.";
          throw runtime_error(ss.str());
        }
      }

      parked_vehicles.push_back(V); 
    }

    template<typename T>
    shared_ptr<T> getParkedVehicle(int index){

      if(T::getClassType()==vehicle::base){
        auto val = dynamic_pointer_cast<T>(parked_vehicles.at(index));
        return val;
      }
      if(T::getClassType()==parked_vehicles.at(index)->getVehicleType()){
        auto val = dynamic_pointer_cast<T>(parked_vehicles.at(index));
        return val;
      }

      stringstream ss;
      ss << "The class you are trying to cast to is of type " << T::getClassType()
         << " whereas the object you are trying to conver from is of type "
         << parked_vehicles.at(index)->getVehicleType()
         << " you can only cast to the base class or to the original object type";
      throw runtime_error(ss.str());

    }

  protected:
    vector<shared_ptr<Vehicle>> parked_vehicles;
    
    list<vehicle> unallowed_vehicle_types_;
};

class TruckGarage : public BaseGarage {
  public:
    TruckGarage() {
      unallowed_vehicle_types_ = {
        vehicle::base,
        vehicle::sedan
      };
    }
};

int main(){

  Vehicle V;
  auto V_ptr = make_shared<Vehicle>(V);

  BaseGarage garage;

  garage.parkVehicle(V_ptr);

  /// Can return a vehicle as it is the base class
  auto V2 = garage.getParkedVehicle<Vehicle>(0);
  assert(V2->getVehicleType()==vehicle::base);

  /// Cannot convert a Vehicle to a Truck as it is a child of Vehicle
  bool convertVehicleToTruck = true;
  try{
    auto V3 = garage.getParkedVehicle<Truck>(0);
  }catch(...){
    convertVehicleToTruck = false; 
  }
  assert(convertVehicleToTruck==false);

  Truck T;
  auto T_ptr = make_shared<Truck>(T);

  garage.parkVehicle(T_ptr);
  
  /// Can return the truck as a vehicle as it is the base class 
  auto T2 = garage.getParkedVehicle<Vehicle>(1);
  assert(T2->getVehicleType()==vehicle::truck);
  
  /// Can return the truck as it was originally a truck
  auto T3 = garage.getParkedVehicle<Truck>(1);
  assert(T3->getVehicleType()==vehicle::truck);

  /// Cannot convert a Truck to a Sedan
  bool convertTruckToSedan = true;
  try{
    auto T4 = garage.getParkedVehicle<Sedan>(1);
  }catch(...){
    convertTruckToSedan = false;
  }
  assert(convertTruckToSedan==false);

  TruckGarage truckgarage;

  bool parkVehicleInTruckGarage = true;
  try{
    truckgarage.parkVehicle(V_ptr);
  }catch(...){
    parkVehicleInTruckGarage = false;
  }
  assert(parkVehicleInTruckGarage==false);

  truckgarage.parkVehicle(T_ptr);

  return 0;
}

