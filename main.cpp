#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <memory>

// Базовый класс для транспортного средства
class Vehicle {
protected:
    std::string id;
    std::string type;
    double mileage;
    bool needsMaintenance;
    double latitude;  // Широта
    double longitude; // Долгота

public:
    Vehicle(const std::string& id, const std::string& type, double mileage)
        : id(id), type(type), mileage(mileage), needsMaintenance(false), latitude(0.0), longitude(0.0) {}

    virtual void displayInfo() const {
        std::cout << "ID: " << id << ", Type: " << type
                  << ", Mileage: " << mileage
                  << ", Maintenance Needed: " << (needsMaintenance ? "Yes" : "No") << std::endl;
        std::cout << "Location: (" << latitude << ", " << longitude << ")\n";
    }

    virtual void performMaintenance() {
        needsMaintenance = false;
        std::cout << "Maintenance completed for vehicle " << id << std::endl;
    }

    virtual ~Vehicle() {}

    const std::string& getID() const { return id; }
    bool isMaintenanceNeeded() const { return needsMaintenance; }
    void setMaintenanceNeeded(bool status) { needsMaintenance = status; }
    double getMileage() const { return mileage; }

    // Методы работы с GPS
    void updateLocation(double lat, double lon) {
        latitude = lat;
        longitude = lon;
        std::cout << "Updated location for " << id << ": (" << latitude << ", " << longitude << ")\n";
    }

    std::pair<double, double> getLocation() const {
        return std::make_pair(latitude, longitude);
    }
};

// Класс для конкретных типов транспортных средств (например, автобусов)
class Bus : public Vehicle {
    int passengerCapacity;

public:
    Bus(const std::string& id, double mileage, int passengerCapacity)
        : Vehicle(id, "Bus", mileage), passengerCapacity(passengerCapacity) {}

    void displayInfo() const {
        Vehicle::displayInfo();
        std::cout << "Passenger Capacity: " << passengerCapacity << std::endl;
    }
};

// Класс для планирования маршрутов
class RoutePlanner {
public:
    static std::string planRoute(const Vehicle& vehicle, const std::string& destination) {
        std::pair<double, double> location = vehicle.getLocation();
        double lat = location.first;
        double lon = location.second;

        std::cout << "Planning route for vehicle " << vehicle.getID()
                  << " from (" << lat << ", " << lon << ") to " << destination << std::endl;

        return "Optimized Route to " + destination;
    }
};

// Класс для анализа данных
class FleetAnalytics {
public:
    static void analyzeEfficiency(const std::vector<std::shared_ptr<Vehicle> >& vehicles) {
        double totalMileage = 0;
        size_t totalVehicles = vehicles.size();
        for (size_t i = 0; i < vehicles.size(); ++i) {
            totalMileage += vehicles[i]->getMileage();
        }
        double avgMileage = totalVehicles > 0 ? totalMileage / totalVehicles : 0;

        std::cout << "Total fleet mileage: " << totalMileage << " km\n";
        std::cout << "Average mileage per vehicle: " << avgMileage << " km\n";
    }
};

// Класс для управления транспортным парком
class FleetManager {
    std::vector<std::shared_ptr<Vehicle> > vehicles;

public:
    void addVehicle(const std::shared_ptr<Vehicle>& vehicle) {
        vehicles.push_back(vehicle);
    }

    void displayAllVehicles() const {
        for (size_t i = 0; i < vehicles.size(); ++i) {
            vehicles[i]->displayInfo();
        }
    }

    void performMaintenance(const std::string& vehicleID) {
        for (size_t i = 0; i < vehicles.size(); ++i) {
            if (vehicles[i]->getID() == vehicleID) {
                vehicles[i]->performMaintenance();
                return;
            }
        }
        std::cout << "Vehicle with ID " << vehicleID << " not found." << std::endl;
    }

    void checkMaintenanceSchedule() const {
        for (size_t i = 0; i < vehicles.size(); ++i) {
            if (vehicles[i]->isMaintenanceNeeded()) {
                std::cout << "Vehicle " << vehicles[i]->getID() << " needs maintenance." << std::endl;
            }
        }
    }

    const std::vector<std::shared_ptr<Vehicle> >& getAllVehicles() const {
        return vehicles;
    }
};

// Основной метод для демонстрации работы системы
int main() {
    FleetManager manager;

    // Добавление транспортных средств
    std::shared_ptr<Bus> bus1 = std::make_shared<Bus>("BUS123", 15000, 50);
    std::shared_ptr<Vehicle> car1 = std::make_shared<Vehicle>("CAR456", "Car", 20000);

    bus1->updateLocation(55.7558, 37.6173); // Москва
    car1->updateLocation(59.9343, 30.3351); // Санкт-Петербург

    manager.addVehicle(bus1);
    manager.addVehicle(car1);

    // Отображение всей информации
    std::cout << "Fleet Information:\n";
    manager.displayAllVehicles();

    // Планирование маршрута
    std::cout << "\nRoute Planning:\n";
    std::cout << RoutePlanner::planRoute(*bus1, "City Center") << std::endl;

    // Выполнение обслуживания
    std::cout << "\nPerforming Maintenance:\n";
    manager.performMaintenance("CAR456");

    // Анализ данных
    std::cout << "\nFleet Analytics:\n";
    FleetAnalytics::analyzeEfficiency(manager.getAllVehicles());

    // Отображение графика обслуживания
    std::cout << "\nMaintenance Check:\n";
    manager.checkMaintenanceSchedule();

    return 0;
}
