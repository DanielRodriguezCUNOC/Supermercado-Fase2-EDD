#include "product.h"

Product::Product(){}

Product::Product(std::string name,
                 std::string barcode,
                 std::string category,
                 std::string expiry_date,
                 std::string brand,
                 double price,
                 int stock) : name(name),
                              barcode(barcode),
                              category(category),
                              expiry_date(expiry_date),
                              brand(brand),
                              price(price),
                              stock(stock)
{
}

// Saber si el producto está en un rango de fechas de caducidad
bool Product::estaEnRangoCaducidad(const std::string &desde, const std::string &hasta) const
{
  // Se asume formato YYYY-MM-DD, por lo que la comparación lexicográfica es válida
  return expiry_date >= desde && expiry_date <= hasta;
}
