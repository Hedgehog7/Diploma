#include "target_selector.h"

#include <QString>
#include <QSqlDatabase>
#include <QException>
#include <QSqlQuery>
#include <QVariant>
#include <QVector>
#include <QtDebug>

#include <iostream>
#include <string>
#include <stdexcept>

namespace NAT {
namespace NMA {

static QStringList PrepareText(const QString& text) {
    QStringList answer;

    for (const QChar& ch : text) {
        if (ch.isSpace()) {
            if (!answer.empty()) {
                answer.push_back(QString());
            }
        } else if (ch.isLetter()) {
            if (answer.empty()) {
                answer.push_back(QString());
            }
            answer.back() += ch.toLower();
        }
    }

    while (!answer.empty() && answer.back().isEmpty()) {
        answer.pop_back();
    }

    return answer;
}

static QVector<double> ParseEmbedding(const QString& text) {
    QVector<double> embedding;

    for (const QString val: text.split(',')) {
        embedding.push_back(val.toDouble());
    }

    return embedding;
}

static double MultEmbeddings(const QVector<double>& a, const QVector<double>& b) {
    if (a.size() != b.size()) {
        return 0;
    }

    double result = 0;
    for (int i = 0; i < a.size(); ++i) {
        result += a[i] * b[i];
    }

    return result;
}

class TTargetSelector::TImpl {
public:
    TImpl(const QString& db) {
        DB_ = QSqlDatabase::addDatabase("QSQLITE");
        DB_.setDatabaseName(db);

        if (!DB_.open()) {
            throw std::logic_error(std::string("Can't open database: ") + db.toUtf8().data());
        }
    }

    bool SelectW2V(const QStringList& tokens, QString& result) const {
        qDebug() << "Run TTargetSelector::TImpl::SelectW2V";

        QMap<QString, QVector<QString>> taskWords;
        {
            QSqlQuery query(DB_);
            query.prepare(
                "SELECT "
                    "word, task "
                "FROM "
                    "main_words"
            );

            if (!query.exec()) {
                throw std::runtime_error("Can't execute main_words query");
            }

            while (query.next()) {
                const QString word = query.value(0).toString();
                const QString task = query.value(1).toString();
                taskWords[task].push_back(word);
            }
        }

        for (const QString task: taskWords.keys()) {
            qDebug() << "Task" << task;
            for (const QString& val: taskWords.value(task)) {
                qDebug() << "  " << val;
            }
        }

        QVector<QString> allWords;
        {
            for (const QString& token: tokens) {
                allWords.push_back(token);
            }
            for (const QString& task: taskWords.keys()) {
                for (const QString& word: taskWords.value(task)) {
                    allWords.push_back(word);
                }
            }
        }

        QMap<QString, QVector<double>> embeddings;
        {
            for (const QString& word: allWords) {
                QSqlQuery query(DB_);
                query.prepare(
                    "SELECT "
                        "embedding "
                    "FROM "
                        "embeddings "
                    "WHERE "
                        "word = '" + word + "'"
                );

                if (!query.exec()) {
                    throw std::runtime_error("Can't execute embeddings query");
                }

                while (query.next()) {
                    const QString embedding = query.value(0).toString();
                    embeddings[word] = ParseEmbedding(embedding);
                }
            }

            qDebug() << "Embeddings";
            for (const QString word: embeddings.keys()) {
                qDebug() << "  " << word;
            }
        }

        {
            for (const QString& token: tokens) {
                if (!embeddings.contains(token)) {
                    continue;
                }

                QMap<QString, double> results;
                for (const QString& task: taskWords.keys()) {
                    for (const QString& word: taskWords.value(task)) {
                        if (!embeddings.contains(word)) {
                            continue;
                        }

                        const double mult = MultEmbeddings(embeddings[word], embeddings[token]);
                        qDebug() << "  MultEmbeddings" << token << word << mult;

                        double& currentResult = results[task];
                        currentResult = qMax(currentResult, mult);
                    }
                }

                QString bestTask;
                for (const QString& task: results.keys()) {
                    const double taskResult = results.value(task);

                    static const double RESULT_THRESHOLD = 0.6;
                    if (taskResult >= RESULT_THRESHOLD) {
                        if (!results.contains(bestTask) || results.value(bestTask) < taskResult) {
                            bestTask = task;
                        }
                    }
                }

                if (results.contains(bestTask)) {
                    result = bestTask;
                    return true;
                }
            }
        }

        return false;
    }

    QString Select(const QString& rawText) const {
        const QStringList tokens = PrepareText(rawText);

        for (const QString& token: tokens) {
            QSqlQuery query(DB_);
            query.prepare(
                "SELECT "
                    "type "
                "FROM "
                    "task_type "
                "WHERE "
                    "word IN ("
                        "SELECT "
                            "word "
                        "FROM "
                            "words "
                        "WHERE "
                            "group_id IN ("
                                "SELECT "
                                    "group_id "
                                "FROM "
                                    "words "
                                "WHERE "
                                    "word = '" + token + "'"
                    ")"
                ")"
            );

            if (!query.exec()) {
                throw std::runtime_error("Can't execute words query");
            }
            while (query.next()) {
                return query.value(0).toString();
            }
        }

        for (const QString& token: tokens) {
            QSqlQuery query(DB_);
            query.prepare(
                "SELECT "
                    "type "
                "FROM "
                    "task_type "
                "WHERE "
                    "word = '" + token + "'"
            );

            if (!query.exec()) {
                throw std::runtime_error("Can't execute query");
            }
            while (query.next()) {
                return query.value(0).toString();
            }
        }

        QString w2v;
        if (SelectW2V(tokens, w2v)) {
            return w2v;
        }

        throw std::runtime_error("Task type not found");
    }

private:
    QSqlDatabase DB_;
};

TTargetSelector::TTargetSelector(const QString& db)
    : Impl_(new TImpl(db))
{
}

QString TTargetSelector::Select(const QString& text) const {
    return Impl_->Select(text);
}

TTargetSelector::~TTargetSelector() = default;

} // namespace NMA
} // namespace NAT
