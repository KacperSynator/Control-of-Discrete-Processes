#pragma once

/*! Struct for holding data about process */
struct RPQ{
    int id; /*!< process id */
    int r;  /*!< release time */
    int p;  /*!< processing time */
    int q;  /*!< delivery time */

    /*! static function for comparing RPQ structures in term of release time (a.r < b.r)*/
    static bool compare_r(RPQ a, RPQ b) {return a.r < b.r;}
    /*! static function for comparing RPQ structures in term of delivery time (a.q < b.q)*/
    static bool compare_q(RPQ a, RPQ b) {return a.q < b.q;}
};