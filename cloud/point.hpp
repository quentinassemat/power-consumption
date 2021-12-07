#pragma once // ensures that this header file is only included once
#include <string>


/**
 * @briefThe point class is used to store the data. Freely inspired from TDs
*/
class point
{   // this is just a declaration of the class
    // implementation is in a separate file: point.cpp
private:
    /**
     * @brief The dimension we are working with
    */
    static int d;

public:
    /**
     * @brief coordinate of the points
    */
    double *coords;
    /**
     * @brief the label of the points ( 0 <= label <= k-1)
    */
    int label;

    /**
     * @brief date of the point
    */
    std::string date;

    /**
     * @brief time of the point
    */
    std::string time;

    /**
     * @brief To set the dimension.
     * @param _d is the value set
    */
    static bool set_dim(int _d);
    /**
     * @brief To get the dimension
     * @return the dimension
    */
    static int get_dim();

    /**
     * @brief the default constructor
    */
    point();

    /**
     * @brief the default destructor
    */
    ~point();

    /**
     * @brief print the point
    */
    void print();

    /**
     * @brief Compute the squared dist to a point
     * @param q is a point to compute distance
     * @return the squared distance
    */
    double squared_dist(point &q);
};
