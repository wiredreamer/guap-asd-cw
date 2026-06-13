#include "menu.h"

#include <iostream>
#include <string>
#include <utility>

#include "model.h"

namespace {

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);
        try {
            return std::stoi(s);
        } catch (...) {
            std::cout << "  Ошибка: введите целое число.\n";
        }
    }
}

void pause() {
    std::cout << "\n[Enter — продолжить] ";
    std::string dummy;
    std::getline(std::cin, dummy);
}

void printPassenger(const Passenger& p) {
    std::cout << "  Номер паспорта: " << p.passportNumber << "\n"
              << "  Выдан:          " << p.passportIssue << "\n"
              << "  ФИО:            " << p.fio << "\n"
              << "  Дата рождения:  " << p.birthDate << "\n";
}

void printFlight(const Flight& f) {
    std::cout << "  Номер рейса:     " << f.flightNumber << "\n"
              << "  Авиакомпания:    " << f.airline << "\n"
              << "  Откуда:          " << f.departureAirport << "\n"
              << "  Куда:            " << f.arrivalAirport << "\n"
              << "  Отправление:     " << f.departureDate << " " << f.departureTime << "\n"
              << "  Мест всего:      " << f.seatsTotal << "\n"
              << "  Мест свободно:   " << f.seatsFree << "\n";
}

void printMenu() {
    std::cout <<  //
        "\n========== Продажа авиабилетов ==========\n"
        " Пассажиры (хеш-таблица, квадратичное опробование):\n"
        "   1. Регистрация нового пассажира\n"
        "   2. Удаление пассажира\n"
        "   3. Просмотр всех пассажиров\n"
        "   4. Очистка данных о пассажирах\n"
        "   5. Поиск пассажира по номеру паспорта\n"
        "   6. Поиск пассажира по ФИО\n"
        "   7. Показать внутреннее устройство хеш-таблицы\n"
        " Авиарейсы (АВЛ-дерево, обратный обход):\n"
        "   8. Добавление авиарейса\n"
        "   9. Удаление авиарейса\n"
        "  10. Просмотр всех авиарейсов\n"
        "  11. Очистка данных об авиарейсах\n"
        "  12. Поиск авиарейса по номеру\n"
        "  13. Поиск авиарейса по фрагменту аэропорта прибытия (Боуера-Мура)\n"
        "  14. Показать обратный (post-order) обход дерева\n"
        " Билеты (слоёный список, сортировка слиянием):\n"
        "  15. Продажа авиабилета\n"
        "  16. Возврат авиабилета\n"
        "  17. Просмотр всех билетов\n"
        "  18. Показать слоёную структуру списка\n"
        " Прочее:\n"
        "  19. Загрузить тестовый набор данных\n"
        "   0. Выход\n"
        "Выбор: ";
}

int readCommand() {
    std::string choice;
    std::getline(std::cin, choice);
    try {
        size_t pos = 0;
        int cmd    = std::stoi(choice, &pos);
        if (pos != choice.size())
            return -1;
        return cmd;
    } catch (...) {
        return -1;
    }
}

}  // namespace

void Menu::registerPassenger() {
    Passenger p;
    p.passportNumber = readLine("Номер паспорта (NNNN-NNNNNN): ");
    if (!isValidPassport(p.passportNumber)) {
        std::cout << "  Ошибка: неверный формат номера паспорта.\n";
        return;
    }
    p.passportIssue = readLine("Место и дата выдачи паспорта: ");
    p.fio           = readLine("ФИО: ");
    p.birthDate     = readLine("Дата рождения: ");

    if (airline_.registerPassenger(p))
        std::cout << "  Пассажир зарегистрирован.\n";
    else
        std::cout << "  Ошибка: пассажир с таким номером паспорта уже есть "
                     "(или таблица переполнена).\n";
}

void Menu::deletePassenger() {
    std::string passport = readLine("Номер паспорта удаляемого пассажира: ");
    int removed          = 0;
    if (airline_.deletePassenger(passport, removed)) {
        std::cout << "  Пассажир удалён.";
        if (removed > 0)
            std::cout << " Аннулировано билетов: " << removed << ".";
        std::cout << "\n";
    } else {
        std::cout << "  Пассажир не найден.\n";
    }
}

void Menu::viewPassengers() const {
    if (airline_.passengers().empty()) {
        std::cout << "  Пассажиров нет.\n";
        return;
    }
    std::cout << "  Зарегистрированные пассажиры:\n";
    for (const Passenger& p : airline_.passengers().items())
        std::cout << "   - " << p.passportNumber << "  " << p.fio << "\n";
}

void Menu::findPassengerByPassport() const {
    std::string passport = readLine("Номер паспорта: ");
    std::vector<std::string> flights;
    const Passenger* p = airline_.findPassenger(passport, flights);
    if (!p) {
        std::cout << "  Пассажир не найден.\n";
        return;
    }
    printPassenger(*p);
    if (flights.empty()) {
        std::cout << "  Купленных билетов нет.\n";
    } else {
        std::cout << "  Билеты на рейсы:\n";
        for (const auto& fn : flights)
            std::cout << "   - " << fn << "\n";
    }
}

void Menu::findPassengerByFio() const {
    std::string fio = readLine("ФИО пассажира: ");
    auto found      = airline_.findPassengersByFio(fio);
    if (found.empty()) {
        std::cout << "  Пассажиры не найдены.\n";
        return;
    }
    std::cout << "  Найдено: " << found.size() << "\n";
    for (const auto& p : found)
        std::cout << "   - " << p.passportNumber << "  " << p.fio << "\n";
}

void Menu::addFlight() {
    Flight f;
    f.flightNumber = readLine("Номер рейса (AAA-NNN): ");
    if (!isValidFlightNumber(f.flightNumber)) {
        std::cout << "  Ошибка: неверный формат номера рейса.\n";
        return;
    }
    f.airline          = readLine("Название авиакомпании: ");
    f.departureAirport = readLine("Аэропорт отправления: ");
    f.arrivalAirport   = readLine("Аэропорт прибытия: ");
    f.departureDate    = readLine("Дата отправления: ");
    f.departureTime    = readLine("Время отправления: ");
    f.seatsTotal       = readInt("Количество мест всего: ");
    if (f.seatsTotal < 0)
        f.seatsTotal = 0;
    f.seatsFree = f.seatsTotal;

    if (airline_.addFlight(f))
        std::cout << "  Авиарейс добавлен.\n";
    else
        std::cout << "  Ошибка: рейс с таким номером уже есть.\n";
}

void Menu::deleteFlight() {
    std::string num = readLine("Номер удаляемого рейса: ");
    int removed     = 0;
    if (airline_.deleteFlight(num, removed)) {
        std::cout << "  Авиарейс удалён.";
        if (removed > 0)
            std::cout << " Аннулировано билетов: " << removed << ".";
        std::cout << "\n";
    } else {
        std::cout << "  Авиарейс не найден.\n";
    }
}

void Menu::viewFlights() const {
    if (airline_.flights().empty()) {
        std::cout << "  Авиарейсов нет.\n";
        return;
    }
    std::cout << "  Авиарейсы (симметричный обход — по возрастанию номера):\n";
    for (const Flight& f : airline_.flights().inorder())
        std::cout << "   - " << f.flightNumber << "  " << f.departureAirport << " -> "
                  << f.arrivalAirport << "  свободно " << f.seatsFree << "/" << f.seatsTotal << "\n";
}

void Menu::showPostorder() const {
    if (airline_.flights().empty()) {
        std::cout << "  Авиарейсов нет.\n";
        return;
    }
    std::cout << "  Обратный (post-order) обход дерева рейсов:\n   ";
    for (const Flight& f : airline_.flights().postorder())
        std::cout << f.flightNumber << " ";
    std::cout << "\n";
}

void Menu::findFlightByNumber() const {
    std::string num = readLine("Номер рейса: ");
    std::vector<std::pair<std::string, std::string>> buyers;
    const Flight* f = airline_.findFlight(num, buyers);
    if (!f) {
        std::cout << "  Авиарейс не найден.\n";
        return;
    }
    printFlight(*f);
    if (buyers.empty()) {
        std::cout << "  Билеты на рейс не проданы.\n";
    } else {
        std::cout << "  Пассажиры с билетами:\n";
        for (const auto& b : buyers)
            std::cout << "   - " << b.first << "  (" << b.second << ")\n";
    }
}

void Menu::findFlightByArrival() const {
    std::string frag = readLine("Фрагмент аэропорта прибытия: ");
    auto found       = airline_.findFlightsByArrivalFragment(frag);
    if (found.empty()) {
        std::cout << "  Авиарейсы не найдены.\n";
        return;
    }
    std::cout << "  Найдено: " << found.size() << "\n";
    for (const auto& f : found)
        std::cout << "   - " << f.flightNumber << "  " << f.arrivalAirport << "  " << f.departureDate
                  << " " << f.departureTime << "\n";
}

void Menu::sellTicket() {
    Ticket t;
    t.ticketNumber = readLine("Номер билета (9 цифр): ");
    if (!isValidTicketNumber(t.ticketNumber)) {
        std::cout << "  Ошибка: номер билета должен состоять из 9 цифр.\n";
        return;
    }
    t.passportNumber = readLine("Номер паспорта пассажира: ");
    t.flightNumber   = readLine("Номер рейса: ");

    switch (airline_.sellTicket(t)) {
        case SellResult::Ok:
            std::cout << "  Билет продан.\n";
            break;
        case SellResult::PassengerNotFound:
            std::cout << "  Ошибка: пассажир не найден.\n";
            break;
        case SellResult::FlightNotFound:
            std::cout << "  Ошибка: авиарейс не найден.\n";
            break;
        case SellResult::NoSeats:
            std::cout << "  Ошибка: нет свободных мест на рейс.\n";
            break;
        case SellResult::DuplicateTicket:
            std::cout << "  Ошибка: билет с таким номером уже существует.\n";
            break;
    }
}

void Menu::returnTicket() {
    std::string num = readLine("Номер возвращаемого билета: ");
    if (airline_.returnTicket(num))
        std::cout << "  Билет возвращён (аннулирован), место освобождено.\n";
    else
        std::cout << "  Билет не найден.\n";
}

void Menu::viewTickets() const {
    if (airline_.tickets().empty()) {
        std::cout << "  Билетов нет.\n";
        return;
    }
    std::cout << "  Билеты (упорядочены по номеру, сортировка слиянием):\n";
    for (const Ticket& t : airline_.tickets().items())
        std::cout << "   - № " << t.ticketNumber << "  паспорт " << t.passportNumber << "  рейс "
                  << t.flightNumber << "\n";
}

void Menu::loadDemo() {
    airline_.clearPassengers();
    airline_.clearFlights();

    airline_.registerPassenger({"4500-100020", "Санкт-Петербург, 12.05.2015", "Иванов Иван Иванович", "01.01.1980"});
    airline_.registerPassenger({"4500-100058", "Москва, 03.07.2018", "Петров Пётр Петрович", "15.03.1975"});
    airline_.registerPassenger({"4500-100255", "Казань, 21.09.2019", "Сидоров Сидор Сидорович", "20.07.1990"});
    airline_.registerPassenger({"4502-200000", "Сочи, 10.02.2020", "Кузнецова Анна Сергеевна", "05.11.1985"});
    airline_.registerPassenger({"4504-400000", "Самара, 30.11.2017", "Смирнов Алексей Олегович", "12.12.2000"});
    airline_.registerPassenger({"4505-500000", "Уфа, 14.06.2016", "Попова Елена Викторовна", "08.08.1995"});

    airline_.addFlight({"SVO-101", "Аэрофлот", "Москва (Шереметьево)", "Санкт-Петербург (Пулково)", "20.06.2026", "08:30", 3, 3});
    airline_.addFlight({"SVO-205", "Аэрофлот", "Москва (Шереметьево)", "Сочи (Адлер)", "20.06.2026", "10:15", 5, 5});
    airline_.addFlight({"LED-310", "Россия", "Санкт-Петербург (Пулково)", "Казань", "21.06.2026", "14:00", 4, 4});
    airline_.addFlight({"AFL-150", "Аэрофлот", "Москва (Домодедово)", "Санкт-Петербург (Пулково)", "22.06.2026", "19:45", 2, 2});
    airline_.addFlight({"UTA-420", "ЮТэйр", "Тюмень", "Сочи (Адлер)", "23.06.2026", "06:20", 6, 6});
    airline_.addFlight({"POB-077", "Победа", "Москва (Внуково)", "Санкт-Петербург (Пулково)", "24.06.2026", "21:10", 4, 4});

    airline_.sellTicket({"4500-100020", "SVO-101", "100000001"});
    airline_.sellTicket({"4500-100058", "SVO-101", "100000002"});
    airline_.sellTicket({"4500-100255", "SVO-205", "100000003"});
    airline_.sellTicket({"4502-200000", "LED-310", "100000004"});
    airline_.sellTicket({"4504-400000", "POB-077", "100000005"});
    airline_.sellTicket({"4505-500000", "UTA-420", "100000006"});
    airline_.sellTicket({"4500-100020", "SVO-205", "100000007"});

    std::cout << "  Тестовый набор загружен: пассажиров " << airline_.passengers().size()
              << ", рейсов " << airline_.flights().size() << ", билетов "
              << airline_.tickets().size() << ".\n";
}

void Menu::run() {
    while (true) {
        printMenu();
        int cmd = readCommand();

        if (cmd == 0)
            break;

        switch (cmd) {
            case 1:
                registerPassenger();
                break;
            case 2:
                deletePassenger();
                break;
            case 3:
                viewPassengers();
                break;
            case 4:
                airline_.clearPassengers();
                std::cout << "  Данные о пассажирах очищены.\n";
                break;
            case 5:
                findPassengerByPassport();
                break;
            case 6:
                findPassengerByFio();
                break;
            case 7:
                airline_.passengers().dump();
                break;
            case 8:
                addFlight();
                break;
            case 9:
                deleteFlight();
                break;
            case 10:
                viewFlights();
                break;
            case 11:
                airline_.clearFlights();
                std::cout << "  Данные об авиарейсах очищены.\n";
                break;
            case 12:
                findFlightByNumber();
                break;
            case 13:
                findFlightByArrival();
                break;
            case 14:
                showPostorder();
                break;
            case 15:
                sellTicket();
                break;
            case 16:
                returnTicket();
                break;
            case 17:
                viewTickets();
                break;
            case 18:
                airline_.tickets().dump();
                break;
            case 19:
                loadDemo();
                break;
            default:
                std::cout << "  Неизвестная команда.\n";
                break;
        }

        pause();
    }
    std::cout << "Завершение работы.\n";
}
