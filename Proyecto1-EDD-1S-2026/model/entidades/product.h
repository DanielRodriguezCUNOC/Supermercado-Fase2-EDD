#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>

class Product
{

private:
    std::string name;
    std::string barcode;
    std::string category;
    std::string expiry_date;
    std::string brand;
    double price;
    int stock;

public:
    Product();
    Product(std::string name,
            std::string barcode,
            std::string category,
            std::string expiry_date,
            std::string brand,
            double price,
            int stock);

    // Saber si el producto está en un rango de fechas de caducidad
    bool estaEnRangoCaducidad(const std::string& desde, const std::string& hasta) const;

    // Getters necesarios para el árbol y CSV
    std::string getName() const { return name; }
    std::string getBarcode() const { return barcode; }
    std::string getCategory() const { return category; }
    std::string getExpiryDate() const { return expiry_date; }
    std::string getBrand() const { return brand; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
};

#endif // PRODUCT_H
