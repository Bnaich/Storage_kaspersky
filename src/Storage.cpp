//
// Created by bnaich on 16.06.19.
//

#include "Storage.h"
#include <iostream>
#include <thread>

void Storage::AddItem(const Item &&item)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_guard);
    article_item_map.emplace(item.article_, item);
    producer_goods_map[item.producer_].emplace(item.article_);
}

bool Storage::GetItemByArticle(const std::string &article, Item &item) const
{
    std::unique_lock<std::recursive_mutex> lock(mutex_guard);
    auto iter = article_item_map.find(article);
    if (iter == article_item_map.end())
        return false;
    item = iter->second;
    return true;
}

Storage::ItemList Storage::GetProducersItems(const std::string &producer) const
{
    std::unique_lock<std::recursive_mutex> lock(mutex_guard);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "\tthread #" << std::this_thread::get_id() << " GETTING " << producer << "`s goods" << std::endl;
    auto iter = producer_goods_map.find(producer);
    if (iter == producer_goods_map.end())
        return ItemList();
    ItemList result_set;
    for (auto &e : iter->second)
        result_set.push_front(article_item_map.at(e));
    return result_set;
}

void Storage::RemoveItemByArticle(const std::string &article)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_guard);
    Item item;
    if (!GetItemByArticle(article, item))
        return;
    auto iter = producer_goods_map.find(item.producer_);
    iter->second.erase(article);

    article_item_map.erase(article);
}

std::size_t Storage::Size() const
{
    return article_item_map.size();
}

Item::Item(std::string article,
           std::string producer,
           std::string title,
           const int64_t cost) : article_(std::move(article)), producer_(std::move(producer)),
                                 title_(std::move(title)), cost_(cost)
{

}

std::ostream & operator<<(std::ostream& os, const Item& item)
{
    os << "Item: " << item.title_ << ", article: " << item.article_ << ", producer: " << item.producer_ << ", cost: " << item.cost_ << std::endl;
    return os;
}
