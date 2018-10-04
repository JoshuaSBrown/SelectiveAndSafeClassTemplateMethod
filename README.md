# SelectiveAndSafeClassTemplateMethod
The contained code simply displays how to create a container which stores a vector of a base class, the container is able to selectively return the contents of the container in any type safely by ensuring that the type is either of base type or is the same type of class that the element was oringinally created as.

I will provide an example to explain. Suppose that we have 3 vehicles:

 * base - base class vehicle type
 * sedan - child of base
 * truck - child of base

Now if I have a container class that stores a vector of the base class, I will call it a garage, I can add any of these three vehicle types to the container with no problems by making use of polymorphism. 

```
class Vehicle {
  public:
    Vehicle() {};
};

class Truck : Vehicle {
  public:
    Truck() {};
};

class Sedan : Vehicle {
  public:
    Sedan() {};
}

class BaseGarage {
  public:
    BaseGarage() {};
    
    void parkVehicle(shared_ptr<Vehicle> V) {
      parked_vehicles.push_back(V);
    }
    
  private:
    vector<shared_ptr<Vehicle>> parked_vehicles;
};

```

The problem comes when I try to get the Vehicles out. Let's assume that I create a vehicle of class Truck and park it in the garage. It will be stored as a pointer to the base class. But how do I get it out as a truck when several vehicle types could be stored in the garage. Well I can use a templated method and dynamically cast to the correct type:

```
class Vehicle {
  public:
    Vehicle() {};
};

class Truck : Vehicle {
  public:
    Truck() {};
};

class Sedan : Vehicle {
  public:
    Sedan() {};
}

class BaseGarage {
  public:
    BaseGarage() {};
    
    void parkVehicle(shared_ptr<Vehicle> V) {
      parked_vehicles.push_back(V);
    }
    
    template<typename T>
    shared_ptr<T> getParkedVehicle(int index){
      return dynamic_pointer_cast<T>(parked_vehicles.at(index));
    }
  private:
    vector<shared_ptr<Vehicle>> parked_vehicles;
};

```

And this is where the problem occurs because say that I put a truck in the garage but I pull I pull it out as a Sedan. Well this code will compile but you end up with a segment fault and no warning. The code in this repo shows how to avoid these segment faults by ensuring that you can only cast to either the base class or the original class that was placed into the container. So the truck could be cast to base or truck with no problems but will throw an error if the user tries to cast it to a sedan. 

## Compile

To compile the code 

```
g++ -o vehicle vehicle.cpp -std=c++11 -Wall -pedantic -Werror
```
