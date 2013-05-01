/*-------------------------------------------------------------------------
 * Utility for printing tables
 *
 * (Currently assumes all values to be printed are doubles.)
 *
 * table_util maintains state with number of columns and number of digits
 * after the decimal point for each column.  These can be set either
 * implicitly by print_headers, or explicitly by the constructor.
 *
 * Column widths are determined by the larger of (1) a min_width parameter
 * provided by caller and either (2a) the width of that column's header
 * plus inter-column padding, or (2b) the distance in characters from the
 * the start of one column header to the next, when all column headers
 * are provided together in a single string.
 *-------------------------------------------------------------------------*/

#ifndef TABLE_UTIL_H
#define TABLE_UTIL_H

#include <cassert>
#include <cstring>

#include <iostream>
#include <iomanip>

#include <algorithm>
#include <vector>

struct table_util {

    size_t cols;                        // number of columns in table
    std::vector<size_t> col_width;      // widths of each column
    static const size_t padding = 4;    // padding between columns

    // Constructors

    table_util() : cols(size_t(0)) {};

    table_util(size_t num_cols, size_t min_width) : cols(num_cols) {
        for (size_t i = 0; i < cols; ++i) {
            col_width.push_back(min_width);
        }
    };

    // Print headers, figuring out column widths by parsing single string of all headers
    // Assumes inter-column padding is part of string

    void print_headers(const char* headerstring) {
        cols = 0;
        char* start = (char*) headerstring;
        char* current = (char*) headerstring;
        while (*current) {
            // skip leading whitespace
            while (*current && isspace(*current)) ++current;
            // go to end of word
            while (*current && !isspace(*current)) ++current;
            col_width.push_back(current - start);
            std::cout << std::string(start, current - start);
            start = current;
            ++cols;
        }
        std::cout << std::endl;
    }

    // Print headers, figuring out column widths from sequence of header strings
    // Adds inter-column padding
    // If a given column width (including padding)is less than min_width, set to min_width

    template <typename I>   // I is InputIterator with value type const char*
    void print_headers(I headers, size_t col_count, size_t min_width) {
        cols = col_count;
        for (size_t i(0); i < cols; ++i, ++headers) {
	    col_width.push_back(strlen(*headers) + (i ? padding : 0)); // no padding before the first column
            col_width[i] = std::max(min_width, col_width[i]);
            std::cout << std::setw(col_width[i]) << *headers;
        }
        std::cout << std::endl;
    }

    // Print headers as above (alternate interface using bounded range)

    template <typename I>   // I is InputIterator
    void print_headers(I hdrs_first, I hdrs_last, size_t min_width) {
        print_headers(hdrs_first, std::distance(hdrs_first, hdrs_last), min_width);
    }

    // Print row of results
    // vals -- iterator in sequence of values to print
    // decimals -- iterator in sequence of number of decimal places to use for each resp. value

    template <typename I1, typename I2> // value type of I1 is double; value type of I2 = size_t
    void print_row(I1 vals, I2 decimals) {
        assert(cols > 0);

        for (size_t i = 0; i < cols; ++i, ++decimals, ++vals) {
            std::cout << std::setw(col_width[i]) << std::fixed << std::setprecision(*decimals) << *vals;
        }        
        std::cout << std::endl;
    }

    // Print row of results, marking winner
    // winner_start -- which column to start with when judging winner
    // max_winner -- if true, use max rather than min to determine winner

    template <typename I1, typename I2>
    void print_row(I1 vals, I2 decimals, size_t winner_start, bool max_winner = false) {
        assert(cols > 0);

        I1 winner = max_winner ? std::max_element(vals + winner_start, vals + cols) 
	                       : std::min_element(vals + winner_start, vals + cols);

        for (size_t i = 0; i < cols; ++i, ++decimals, ++vals) {
            size_t width = (i < winner_start) ? col_width[i] : col_width[i] - 2; 
            std::cout << std::setw(width) << std::fixed << std::setprecision(*decimals) 
                      << *vals << (vals == winner ? " *" : "  ");
        }        
        std::cout << std::endl;
    }
};

#endif
