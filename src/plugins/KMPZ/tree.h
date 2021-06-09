#ifndef TREE_H
#define TREE_H

#include <QString>
#include <QVector>


class InterviewTreeNode {
private:
    QString step;
    QVector<InterviewTreeNode*>* children;

public:
    InterviewTreeNode* parent;
    InterviewTreeNode(QString step) {
        this->step = step;
        this->children = new QVector<InterviewTreeNode*>();
    };;
    ~InterviewTreeNode() {
        delete this->children;
    };;
    void AddChild(InterviewTreeNode* node) {
        node->parent = this;
        this->children->push_back(node);
    };

    int ChildrenCount() {
        return this->children->count();
    };
    void RemoveChildByIndex(int index) {
        InterviewTreeNode* tmp = this->children->operator[](index);
        tmp->parent = NULL;
        this->children->remove(index);
        delete tmp;
    };

    QString toHTML() {
        QString nodeClassName = "node-content";
        QString labelClassName = "node-name";
        if (!this->children->count()) {
            labelClassName += " conclusion";
        }
        QString html = "<div class=\"" + nodeClassName + "\"><label class=\"" + labelClassName + "\">" + this->step + "</label>";
        for (int i = 0; i < this->children->count(); i++) {
            html += this->children->operator[](i)->toHTML();
        }
        return html + "</div>";
    };
    QString toTXT() {
        return "";
    };
};

class InterviewTree {
private:
    InterviewTreeNode* root;
    //static InterviewTree* t_instance;

    //InterviewTree(const InterviewTree&) = delete;
    //InterviewTree& operator=(InterviewTree&) = delete;

public:

//    ~InterviewTree();
    //static InterviewTree* getInstance(QString step) {
    //    if (!t_instance)
    //        t_instance = new InterviewTree(step);
    //    return t_instance;
    // };
    InterviewTree(QString step) {
        this->root = new InterviewTreeNode(step);
    };

    ~InterviewTree() {
        delete this->root;
    }
    InterviewTreeNode* GetRoot() {
        return this->root;
    };

    QString toHTML() {
        QString html = "<html>"
            "<head>"
            "<style>"
                ".node-content {"
                    "padding-left: 20px;"
                    "border-left: 1px dotted black;"
                "}"
                ".node-name {cursor: pointer}"
                ".node-name:hover {color: blue}"
                ".node-name:before {"
                    "content: \" \";"
                    "display:block;"
                    "position: relative;"
                    "width: 20px;"
                    "left: -20px;"
                    "top: 10px;"
                    "border-top: 1px dotted black;"
                "}"
                ".conclusion {"
                    "background: yellow;"
                "}"
            "</style>"
            "</head>"
            "<body>" + this->root->toHTML() +
            "<script>"
                "Array.from(document.getElementsByClassName('node-name')).map(label => {"
                    "label.visible = true;"
                    "label.onclick = () => {"
                        "let content = label.parentNode.getElementsByClassName('node-content')[0];"
                        "while (content) {"
                            "content.style.display = label.visible ? 'none' : 'block';"
                            "content = content.nextElementSibling;"
                        "}"
                        "label.visible = !label.visible;"
                    "}"
                "})"
            "</script>"
            "</body>"
            "</html>";
        return html;
    };
    QString toTXT() { return ""; };
};

//namespace TreeName { extern InterviewTree* myTree; }
//namespace  {  InterviewTree* myTree; }
//extern InterviewTree* myTree;

//InterviewTree::~InterviewTree()
//{
//    delete this->root;
//}

//InterviewTree* InterviewTree::t_instance;

#endif // TREE_H
