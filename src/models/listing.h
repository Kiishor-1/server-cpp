#ifndef LISTING_H
#define LISTING_H

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <string>
#include <stdexcept>
#include <vector>
#include <unordered_set> // For the allowed categories set

class Listing {
public:
    // Member variables for the listing
    std::string owner; // This will be set separately during creation
    std::string country;
    std::string location;
    double price;
    double priceAfterTax; 
    std::string title;
    std::string description;
    std::string category; // New field for category
    struct Image {
        std::string url;
        std::string filename;
    } image; // Struct for image
    std::vector<std::string> reviews; // Vector for reviews
    struct Geometry {
        std::string type; // Should be "Point"
        std::vector<double> coordinates; // Latitude and Longitude
    } geometry; // Geometry field to hold coordinates and type

    // Constructor
    Listing(const std::string &owner, const std::string &country, const std::string &location,
            double price, double priceAfterTax,  const std::string &title, const std::string &description,
            const std::string &category, const std::string &image_url, const std::string &image_filename,
            const Geometry &geometry);

    // Static method to create a Listing from JSON
    static Listing fromJson(const crow::json::rvalue &json_data);

    // Method to convert Listing to BSON
    bsoncxx::document::value toBson() const;

    // Method to validate Listing fields
    void validate() const;

    // Static method to check if the category is valid
    static bool isValidCategory(const std::string& category) {
        return allowed_categories.find(category) != allowed_categories.end();
    }

private:
    // Allowed categories (private for encapsulation)
    static const std::unordered_set<std::string> allowed_categories;
};

#endif // LISTING_H

