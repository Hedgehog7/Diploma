#ifndef VERFUNC_H
#define VERFUNC_H

#include<QFile>
#include<QString>
#include<QVector>

int find_temporal_markers(QFile &file);

int find_same_rules(QFile &file);

int find_void_conclusions(QFile &file);

int find_void_conditions(QFile &file);

#endif // VERFUNC_H
