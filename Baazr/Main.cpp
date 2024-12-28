#include <Calculator/calc.h>
#include <h_curl/h_curl.h>

int main( ) {
    Calculator c;
    H_Curl cInstance;

    Utils::InitializeConsole( );

    bool online = Utils::checkSkyblockBazaarStatus( );

    if (online) {
        Utils::setConsoleTextColor( 2 );
        std::cout << " Skyblock Bazaar API [Online]" << std::endl;
        Utils::setConsoleTextColor( 7 );
    }
    else {
        Utils::setConsoleTextColor( 4 );
        std::cout << " Skyblock Bazaar API [Offline]" << std::endl;
        Sleep( 5000 );
        return 1;
    }

    std::cout << "\n Please wait a few seconds..!" << std::endl;
    Sleep( 1000 );

    while (true) {
        std::string Item;

        Utils::clearConsole( );

        std::cout << "\n\n Please enter a valid ItemID [>] ";
        std::cin >> Item;

        Utils::clearConsole( );

        int Amount;

        std::cout << "\n\n Please enter the Amount of Items [>] ";
        std::cin >> Amount;

        std::string response = cInstance.m_request( "https://api.hypixel.net/skyblock/bazaar" );

        if (response.empty( )) {
            std::cerr << "Failed to get a valid response from the Hypixel API." << std::endl;
            return 1;
        }

        try {
            nlohmann::json jsonResponse = nlohmann::json::parse( response );

            if (jsonResponse.contains( "products" ) && jsonResponse["products"].is_object( )) {
                if (jsonResponse["products"].contains( Item )) {
                    const auto& productDetails = jsonResponse["products"][Item];

                    if (productDetails.contains( "quick_status" ) && productDetails["quick_status"].is_object( )) {
                        const auto& quickStatus = productDetails["quick_status"];

                        double sellPrice = quickStatus["buyPrice"];
                        double buyPrice = quickStatus["sellPrice"];

                        std::string fixedSell = c.formatRevenue( sellPrice );
                        std::string fixedBuy = c.formatRevenue( buyPrice );

                        Utils::clearConsole( );

                        std::cout << "\n\nBuy-price [>] " << fixedSell << "        ";
                        std::cout << "Sell-price [>] " << fixedBuy << "        ";

                        c.calculateProfit( buyPrice, Amount, sellPrice );
                    }
                    else {
                        std::cerr << "\n\n[>] No Information found for the product." << std::endl;
                    }
                }
                else {
                    std::cerr << "\n\n[>] Product with ID '" << Item << "' not found in the API response." << std::endl;
                }
            }
            else {
                std::cerr << "\n\n[>] No 'products' field found in the response." << std::endl;
            }
        }
        catch (const nlohmann::json::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what( ) << std::endl;
            return 1;
        }

        std::cout << "\n\nPress Enter to check another item or Ctrl+C to exit.";
        std::cin.ignore( );
        std::cin.get( );
    }

    return 0;
}