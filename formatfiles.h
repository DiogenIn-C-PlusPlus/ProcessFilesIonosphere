#pragma once

#include <string>

struct FormatFileMof
{
    std::string trace;
    std::string data;
    std::string time;
    double hours;
    size_t seconds;
    std::string unknown_data;
    double max_objective_freq;
    std::string unknown_data_sec;
    double time_delay;
};

struct FormatFilePrognosticRadioRouteAustralia
{
    double time;
    double time_persent;
    double freq_persent_mof;
    double amplitude_1;
    double amplitude_2;
    double amplitude_3;
    double amplitude_4;
    double amplitude_5;
    double amplitude_6;
    double freq;
    double unknow_time;
    size_t count_hops;
    double freq_hop_1;
    double freq_hop_2;
    double freq_hop_3;
    double freq_hop_4;
    double freq_hop_5;
    double freq_hop_6;
    double freq_hop_7;
    double freq_hop_8;
    double freq_hop_9;
    double freq_hop_10;
    double freq_hop_11;
    double freq_hop_12;
    double freq_hop_13;
    double freq_hop_14;
    double freq_hop_15;
    double freq_hop_16;
    double freq_hop_17;
    double freq_hop_18;
    double freq_hop_19;
    double freq_hop_20;
    double max_freq_hop;
    double freq_hop_22;
    double freq_hop_23;
    double unknow_data;
};

struct FormatFilePrognosticRadioRouteCyprus
{
    double time;
    double time_persent;
    double freq_persent_mof;
    double amplitude_1;
    double amplitude_2;
    double amplitude_3;
    double amplitude_4;
    double amplitude_5;
    double amplitude_6;
    double freq;
    double unknow_time;
    size_t count_hops;
    double freq_hop_1;
    double freq_hop_2;
    double freq_hop_3;
    double freq_hop_4;
    double max_freq_hop;
    double freq_hop_5;
    double freq_hop_6;
    double unknow_data;
};
