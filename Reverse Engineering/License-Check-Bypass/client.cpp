#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>

int main() {
    std::string cle;
    std::cout << "--> Entrez votre clef : ";
    std::cin >> cle;

    // JSON à envoyer
    std::string json = "{\"cle\":\"" + cle + "\"}";

    // Initialisation
    HINTERNET hInternet = InternetOpenA("WinINetClient", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "Erreur InternetOpenA\n";
        return 1;
    }

    // Connexion HTTP
    HINTERNET hConnect = InternetConnectA(hInternet, "127.0.0.1", 5000, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        std::cerr << "Erreur InternetConnectA\n";
        InternetCloseHandle(hInternet);
        return 1;
    }

    // Préparation de la requête POST
    const char* headers = "Content-Type: application/json";
    HINTERNET hRequest = HttpOpenRequestA(
        hConnect, "POST", "/verifier", NULL, NULL, NULL,
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0
    );

    if (!hRequest) {
        std::cerr << "Erreur HttpOpenRequestA\n";
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // Envoi de la requête
    if (!HttpSendRequestA(hRequest, headers, strlen(headers), (LPVOID)json.c_str(), json.length())) {
        std::cerr << "Erreur HttpSendRequestA\n";
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // Lecture de la réponse
    char buffer[1024];
    DWORD bytesRead = 0;
    std::string response;

    std::cout << "\n--> Reponse du serveur :\n";
    while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::cout << buffer;
        response += buffer;
    }

    // Vérifie si la clé est valide
    if (response.find("true") != std::string::npos) {
        std::cout << "\n\n----> Licence validee ! Lancement du programme...\n";

        // Simule une interface "débloquée"
        std::cout << R"(
    =====================================
    | LOGICIEL "CarotteStudio" DEBLOQUE |
    |   Bienvenue dans la version pro   |
    |      - Option 1 : Sauvegarder     |
    |      - Option 2 : Importer 	    |
    |      - Option 3 : Play	        |
    |				                    |
    =====================================
    )";
    } else {
        std::cout << "\n Clef invalide. Programme bloque.\n";
    }

    // Nettoyage
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    std::cout << "\n\nAppuyez sur Entree pour quitter...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}
