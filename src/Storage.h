//
// Created by bnaich on 16.06.19.
//

#ifndef KASPERSKY_STORAGE_STORAGE_H
#define KASPERSKY_STORAGE_STORAGE_H

#include <string>
#include <unordered_map>
#include <set>
#include <forward_list>
#include <mutex>

struct Item
{
    Item() = default;
    Item(std::string  article, std::string  producer, std::string  title, int64_t cost);
    std::string article_;
    std::string producer_;
    std::string title_;
    int64_t     cost_;
    friend std::ostream & operator<<(std::ostream& os, const Item& item);
};

struct Storage
{
    using StorageArticleItemMap = std::unordered_map <std::string, Item>;
    using ArticleSet = std::set<std::string>;
    using ItemList = std::forward_list<Item>;
    using ProducerGoodsMap = std::unordered_map<std::string, ArticleSet>;

    void AddItem(const Item&& item);
    bool GetItemByArticle(const std::string& article, Item& item) const;
    ItemList GetProducersItems(const std::string& producer) const;
    void RemoveItemByArticle (const std::string& article);

    std::size_t Size() const;
private:
    StorageArticleItemMap		    article_item_map;
    ProducerGoodsMap				producer_goods_map;
    mutable std::recursive_mutex    mutex_guard;
};


#endif //KASPERSKY_STORAGE_STORAGE_H