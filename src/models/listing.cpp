#include "listing.h"
#include <unordered_set> // For the allowed categories set
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/exception/exception.hpp>

// Define the allowed categories
const std::unordered_set<std::string> Listing::allowed_categories = {
    "Trending", "Rooms", "Mountains", "Farms", "Castle", "Pools",
    "Camping", "Beachfront", "Arctic", "Tropicals", "Lake"};

// Constructor
Listing::Listing(const std::string &owner, const std::string &country, const std::string &location,
                 double price, double priceAfterTax, const std::string &title, const std::string &description,
                 const std::string &category, const std::string &image_url, const std::string &image_filename,
                 const Geometry &geometry)
    : owner(owner), country(country), location(location), price(price), priceAfterTax(priceAfterTax),
      title(title), description(description), category(category), reviews({}), // Initialize with empty reviews
      image{image_url, image_filename}, geometry(geometry)                     // Initialize geometry
{
}

// Validation method
void Listing::validate() const
{
    if (country.empty())
        throw std::invalid_argument("Country cannot be empty");
    if (location.empty())
        throw std::invalid_argument("Location cannot be empty");
    if (price <= 0)
        throw std::invalid_argument("Price must be greater than 0");
     if (priceAfterTax < price)
        throw std::invalid_argument("Price after tax cannot be less than the base price");
    if (title.empty())
        throw std::invalid_argument("Title cannot be empty");
    if (description.empty())
        throw std::invalid_argument("Description cannot be empty");

    // Validate the category field
    if (category.empty())
        throw std::invalid_argument("Category cannot be empty");
    if (!isValidCategory(category))                                         // Using the static method to validate category
        throw std::invalid_argument("Invalid category value: " + category); // Check if category is valid

    if (image.url.empty())
        throw std::invalid_argument("Image URL cannot be empty");
    if (image.filename.empty())
        throw std::invalid_argument("Image filename cannot be empty");
    if (geometry.coordinates.size() != 2)
        throw std::invalid_argument("Geometry coordinates must contain 2 elements");
}

// Convert to BSON for MongoDB
bsoncxx::document::value Listing::toBson() const
{
    bsoncxx::builder::stream::document doc{};

    // Build the BSON document with proper structure
    doc << "country" << country
        << "location" << location
        << "price" << price
        << "priceAfterTax" << priceAfterTax
        << "title" << title
        << "description" << description
        << "category" << category // Add category to BSON
        << "image" << bsoncxx::builder::stream::open_document
        << "url" << image.url
        << "filename" << image.filename
        << bsoncxx::builder::stream::close_document
        << "geometry" << bsoncxx::builder::stream::open_document
        << "type" << geometry.type
        << "coordinates" << bsoncxx::builder::stream::open_array
        << geometry.coordinates[0] << geometry.coordinates[1]
        << bsoncxx::builder::stream::close_array
        << bsoncxx::builder::stream::close_document;

    // Open the array context for reviews
    bsoncxx::builder::stream::array reviews_array;
    for (const auto &review : reviews)
    {
        reviews_array << review; // Add each review to the array
    }

    // Insert the reviews array into the document
    doc << "reviews" << reviews_array;

    // Finalize and return the BSON document
    return doc << bsoncxx::builder::stream::finalize;
}

// Static method to create a Listing object from JSON
Listing Listing::fromJson(const crow::json::rvalue &json_data) {
    std::cout << "Full JSON received in fromJson(): " << json_data << std::endl;  // Log full JSON


    // Check if "geometry" exists before accessing it
    if (!json_data.has("geometry") || !json_data["geometry"].has("coordinates")) {
        throw std::invalid_argument("Geometry or coordinates missing");
    }

    Geometry geometry = {
        "Point",                                                                                   // Geometry type
        {json_data["geometry"]["coordinates"][0].d(), json_data["geometry"]["coordinates"][1].d()} // Extract coordinates
    };

    double price = json_data["price"].d();  // Ensure price is extracted
    double priceAfterTax;

    // Check if "priceAfterTax" exists, otherwise calculate 18% more than price
    if (json_data.has("priceAfterTax")) {
        priceAfterTax = json_data["priceAfterTax"].d();
    } else {
        priceAfterTax = price * 1.18;  // Calculate 18% more than price
        std::cout << "Calculated priceAfterTax: " << priceAfterTax << std::endl;
    } 

    std::string category;
    try {
        // Check if category exists in JSON
        if (!json_data.has("category")) {
            throw std::invalid_argument("Category key missing from JSON");
        }

        category = json_data["category"].s(); // Assign category
    } catch (const mongocxx::exception &e) {
        std::cout << "Failed: " << e.what() << std::endl;
        throw std::invalid_argument("Failed to extract category: " + std::string(e.what()));
    } catch (const std::exception &e) {
        std::cout << "Failed: " << e.what() << std::endl;
        throw std::invalid_argument("Failed to extract category: " + std::string(e.what()));
    }


    if (!isValidCategory(category)) {
        throw std::invalid_argument("Invalid category value: " + category);
    }


    return Listing(
        "", // Owner will be set separately in handleCreateListing
        json_data["country"].s(),          // Check if "country" exists
        json_data["location"].s(),         // Check if "location" exists
        price,                              // Check if "price" exists
        priceAfterTax,
        json_data["title"].s(),            // Check if "title" exists
        json_data["description"].s(),      // Check if "description" exists
        category,                          // Pass category
        json_data["image"]["url"].s(),     // Check if "image" and "url" exist
        json_data["image"]["filename"].s(),// Check if "image" and "filename" exist
        geometry                           // Pass geometry
    );
}
