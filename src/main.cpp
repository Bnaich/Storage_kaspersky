#include <iostream>
#include <future>
#include <chrono>
#include <cassert>
#include "Storage.h"

void create_some_goods(Storage& s, std::string& producer, int from, int to)
{
    for (int i = from; i <= to; ++i)
    {
        auto good_article = "good_" + std::to_string(i);
        auto good_price = rand() % 1000 + 100;
        s.AddItem(Item(std::to_string(i), producer, good_article, good_price));
        std::cout << "\tThread #" << std::this_thread::get_id() << " ADD " << good_article << " from " << producer << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void remove_some_goods(Storage& s, const Storage::ItemList& item_list)
{
    for (auto& item : item_list)
    {
        s.RemoveItemByArticle(item.article_);
        std::cout << "\tThread #" << std::this_thread::get_id() << ' ' <<  item.title_ << " REMOVED" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}



int main()
{
    std::cout << "Thread #" << std::this_thread::get_id() << ": Storage creating..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Storage s;
    std::cout << "Thread #" << std::this_thread::get_id() << ": Storage filling START..." << std::endl;
    auto first_producer = std::string("Abibas");
    auto second_producer = std::string("Mike");


    auto task1 = std::async(std::launch::async, create_some_goods, std::ref(s), std::ref(first_producer), 1, 10);
    auto task2 = std::async(std::launch::async, create_some_goods, std::ref(s), std::ref(second_producer), 11, 20);
    task1.get();
    task2.get();
    assert(s.Size() == 20);

    std::cout << "Thread #" << std::this_thread::get_id() << ": Storage filling STOP..." << std::endl;

    std::cout << "Thread #" << std::this_thread::get_id() << ": START queries..." << std::endl;
    auto task3 = std::async(std::launch::async, &Storage::GetProducersItems, std::ref(s), std::ref(first_producer));
    auto task4 = std::async(std::launch::async, &Storage::GetProducersItems, std::ref(s), std::ref(second_producer));

    auto AbibasGoods = task3.get();
    auto MikeGoods = task4.get();
    std::cout << "Thread #" << std::this_thread::get_id() << ": STOP queries..." << std::endl;

    std::cout << "Thread #" << std::this_thread::get_id() << ": START requests by_id..." << std::endl;
    Item item;
    s.GetItemByArticle("7", item);
    std::cout << '\t' << item;
    std::cout << "Thread #" << std::this_thread::get_id() << ": STOP  requests by_id..." << std::endl;


    std::cout << "Thread #" << std::this_thread::get_id() << ": START removing goods..." << std::endl;
    auto task5 = std::async(std::launch::async, remove_some_goods, std::ref(s), std::ref(AbibasGoods));
    auto task6 = std::async(std::launch::async, remove_some_goods, std::ref(s), std::ref(MikeGoods));
    task5.get();
    task6.get();
    std::cout << "Thread #" << std::this_thread::get_id() << ": STOP  removing goods..." << std::endl;

    assert(s.Size() == 0);


    std::cout << "Thread #" << std::this_thread::get_id() << " PRESS any key to continue..." << std::endl;

    std::cin.ignore();
    return 0;
}