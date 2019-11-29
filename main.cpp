#include <iostream>
#include <string_view>
#include <vector>

static struct node * head;

enum class contain_t {
    begin,
    end,
    no
};

struct node {
    node(std::string_view str) : label(str) {}
    std::string_view label = "";
    std::vector<node*> childs;

    contain_t add(std::string_view text) {
        auto lb = label.size() != 0;
        auto ch = childs.size() > 0;

        auto func = [this](std::string_view & suf) {
            //для каждого из потомков вызываем метод.
            //Если не найдутся, то добавляем ещё одним потомком
            for(auto & c : childs) {
                auto res = c->add(suf);
                //есть в начале
                if(res == contain_t::begin) {
                    auto tmp = new node(suf);
                    //вычислить остаток
                    c->label = c->label.substr(suf.size(), label.size() - suf.size());
                    tmp->childs.push_back(c);
                    c = tmp;
                    return contain_t::begin;
                }
                //есть в конце
                else if(res == contain_t::end) {
                    return contain_t::end;
                }
                //нет совпадений
                else if(res == contain_t::no) {}
            }
            //среди потомков не нашлось подходящего. Добавляем себе.
            this->childs.push_back(new node(suf));
            return contain_t::no;
        };

        if(lb) {
            //Есть текст и потомки
            if(ch) {
                /* сравниваем полученный и текущий тексты.
                 * Если text короче и содержится в начале label, то он для нас родитель.
                 * Если label короче и содержится в начале text, мы для него родитель.
                 *   Переходим к проверке потомков. Оставляем только суффикс text.
                 *   Если суффикс содержится в начале потомка, то он для него родитель. Вставляем.
                 *   Если потомок содержится в начале суффикса, то потомок родитель для него. Переходим к его потомкам.
                 *
                 * text длиннее.
                 * проверить потомков. Если ни в одном нет ничего, то добавить себе.
                 * Если есть, то...
                 */

                //text содержится в начале label
                if(text.size() < label.size() && label.find_first_of(text) == 0) {
//                    //создаем нового родителя с текстом text
//                    auto nd = new node(text);
//                    //У потомка оставляем суффикс
//                    label = label.substr(text.size(), label.size() - text.size());
//                    //возвращаем нового родителя
                    return contain_t::begin;
                }
                //label содержится в начале text
                else if(label.size() < text.size() && text.find_first_of(label) == 0) {
                    //получаем суффикс от text
                    auto suf = text.substr(label.size(), text.size() - label.size());
                    func(suf);
                }
            }
            //Есть текст, нет потомков
            else {
                //если содержится в начале, создаем потомка
                //если содержится в конце, возвращаем end.
            }
        } else {
            //Нет текста, есть потомки
            if(ch) {
                return func(text);
            }
            //Нет ни текста, ни потомков
            else {
                label = text;
                return contain_t::no;
            }
        }
        return contain_t::no;
    }    
};

int main()
{
    head = new node("");
    for(std::string line; std::getline(std::cin, line); )
    {
        head->add(line);
    }
    return 0;
}
