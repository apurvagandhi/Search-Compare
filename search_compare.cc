/***********************************************************
File name: search_compare.cc
Name: Apurva Gandhi
Assignment: Assignment 7
Date: 04/11/2019
CSCI132
Professor: Saba Kadady
Purpose: Run and calculate the average comparision to search
a target using 2 different search methods: sequential search,
and Ternary Search
***********************************************************/
enum Error_code { success, fail, range_error, underflow, overflow, fatal,
                  not_present, duplicate_error, entry_inserted, entry_found,
                  internal_error };

#include <iostream>
#include <cstdlib>
#include "list.h"
#include "list.cc"
#include "key.h"
#include "key.cc"
using namespace std;

typedef Key Record;
void test_search(int, List<Record> &);
int Randomize(int);
Error_code sequential_search(const List<Record> &,
                             const Key &, int &);
Error_code recursive_binary_1(const List<Record> &, const Key &, 
                              int, int, int &);
Error_code run_recursive_search_1(const List<Record> &,
                                  const Key &, int &);
Error_code ternary(const List<Record> &, const Key &, 
                   int, int, int &);
Error_code run_ternary(const List<Record> &,
                       const Key &, int &);


int main()
{
    int list_size = 0, searches = 100;
    cout << "Please enter a list size: " << endl;
    cin >> list_size;
    cout << "Timing and testing of search algorithms on a list of size " << list_size << endl;
    cout << endl;
  
    int i;
    List<Record> the_list;
    for (i = 0; i < list_size; i++)
        if (the_list.insert(i, 2 * i + 1) != success)
        {
            cout << " Overflow in filling list." << endl;
        }
    test_search(searches, the_list);
} //end main

/*Testing program:*/
void print_out(string comment, string searchType, int comp_ct, int searches)
{
    float average;
    cout << comment << " " << searchType << " Search Statistics: " << endl;
    cout << "Total number of comparisons was " << comp_ct << endl;
    average = (( float )comp_ct)/ (( float )searches) ;
    cout << " Average number of comparisons per search was " << average << endl;
    cout << endl;

} //print_out

void test_search(int searches, List<Record> &the_list)
/*
  Pre: None.
  Post: The number of key comparisons and CPU time for a
  sequential searching funtion
  have been calculated.
  Uses: Methods of the classes List, Random, and Timer,
  together with an output function print_out
*/
{
    int list_size = the_list.size();
    if (searches <= 0 || list_size < 0)
    {
        cout << " Exiting test: " << endl
             << " The number of searches must be positive." << endl
             << " The number of list entries must exceed 0." << endl;
        return;
    } //robust check
  
    int i, target, found_at;

    //Testing for successful Sequential search
    Key::comparisons = 0;
    for (i = 0; i < searches; i++)
    {
        target = 2 * Randomize(list_size - 1)+ 1;
        if (sequential_search(the_list, target, found_at)== not_present)
            cout << "Error: Failed to find expected target " << target << endl;
    } 
    print_out("Successful", "Sequential",  Key::comparisons, searches);

    //Testing for successful Ternary search
    Key::comparisons = 0;
    for (i = 0; i < searches; i++)
    {
        target = 2 * Randomize(list_size - 1)+ 1;
        if (run_ternary(the_list, target, found_at)== not_present)
            cout << "Error: Failed to find expected target " << target << endl;
    } 
    print_out("Successful", "Ternary", Key::comparisons, searches);

    //Testing for unsuccessful Sequential search
    Key::comparisons = 0;
    for (i = 0; i < searches; i++)
    {
        target = 2 * Randomize(list_size);
        if (sequential_search(the_list, target, found_at)== success)
            cout << "Error: Found unexpected target " << target
                 << " at " << found_at << endl;
    }
    print_out("Unsuccessful", "Sequential", Key::comparisons, searches);

    //Testing for unsuccessful Ternary search
    Key::comparisons = 0;
    for (i = 0; i < searches; i++)
    {
        target = 2 * Randomize(list_size);
        if (run_ternary(the_list, target, found_at)== success)
            cout << "Error: Found unexpected target " << target
                 << " at " << found_at << endl;
    }
    print_out("Unsuccessful", "Ternary", Key::comparisons, searches);

} //test_search

Error_code sequential_search(const List<Record> &the_list,
                             const Key &target, int &position)
/*
  Post: If an entry in the_list has key equal to target, then return
  success and the output parameter position locates such an entry
  within the list.
  Otherwise return not_present and position becomes invalid.
*/
{
    int s = the_list.size();
    for (position = 0; position < s; position++)
    {
        Record data;
        the_list.retrieve(position, data);
        if ((Key)data == target)return success;
    }
    return not_present;
}//end of sequential_search

Error_code run_recursive_search_1(const List<Record> &the_list,
                                  const Key &target, int &position)
/*
  Post: If an entry in the_list has key equal to target, then return
  success and the output parameter position locates such an entry
  within the list.
  Otherwise return not_present and position becomes invalid.
*/
{
    return recursive_binary_1(the_list, target, 0, the_list.size() -1, position);
} //run_recursive_search_1

Error_code recursive_binary_1(const List<Record> &the_list, const Key &target, 
                              int bottom, int top, int &position)
/* Pre: The indices bottom to top define the range in the list to search 
   for the target.
   Post: If a Record in the range of locations from bottom to top in the_list 
   has key equal to target, then position locates one such entry and a code of
   success is returned. Otherwise, the Error_code of not_present is returned
   and position becomes undefined.
   Uses: recursive_binary_1 and methods of the classes List and Record. */
{
    Record data;
    if (bottom < top) // List has more than one entry.
    {
        int mid = (bottom + top)/2;
        the_list.retrieve(mid, data);
        if (data < target) // Reduce to top half of list.
            return recursive_binary_1(the_list, target, mid + 1, top, position);
        else // Reduce to bottom half of list.
            return recursive_binary_1(the_list, target, bottom, mid, position);
    } else if (top < bottom)
        return not_present; // List is empty.
    else
    { // List has exactly one entry.
        position = bottom;
        the_list.retrieve(bottom, data);
        if (data == target) return success;
        else return not_present;
    }
}//end of recursive_binary_1


Error_code run_ternary(const List<Record> &the_list,
                       const Key &target, int &position)
/*
  Post: If an entry in the_list has key equal to target, then return
  success and the output parameter position locates such an entry
  within the list.
  Otherwise return not_present and position becomes invalid.
*/
{
    return ternary(the_list, target, 0, the_list.size() - 1, position);
} //end of run_ternary

Error_code ternary(const List<Record> &the_list, const Key &target, 
                   int bottom, int top, int &position)
/* Pre: The indices bottom to top define the range in the list to search 
   for the target.
   Post: If a Record in the range of locations from bottom to top in the_list 
   has key equal to target, then position locates one such entry and a code of
   success is returned. Otherwise, the Error_code of not_present is returned
   and position becomes undefined.
   Uses: ternary and methods of the classes List and Record. */
{
    Record data;
    if (bottom <= top)
    { // List has more than one entry.
        int one_third = (bottom + (top-bottom)/3);
        int two_thirds = (bottom + 2*(top-bottom)/3);

        the_list.retrieve(one_third, data);
        //data at the 1/3 of the list
        if(data == target)
        {
            position = one_third;
            return success;
        }
        //target is bigger than data at 1/3 of list, it will look into other 2/3 of the list
        if(data < target)
        {
            the_list.retrieve(two_thirds, data);
            if(data == target)
            {
                position = two_thirds;
                return success;
            }
            //target is bigger than data at 2/3 of list, it will look into end - third 1/3 of the list to end
            else if(data < target)
            {
                return ternary(the_list, target, two_thirds + 1, top, position);
            }
            //target is smaller than data at 2/3 of list, it will look into middle (second) 1/3 of the list to end
            else
            {
                return ternary(the_list, target, one_third + 1,  two_thirds - 1, position);
            }
        }
        else //target is smaller than data at 1/3 of list, it will look the first 1/3 of the list
            return  ternary(the_list, target, bottom, one_third - 1, position);
    }
    return not_present;
} //end of ternary

/**********Randomize*****************************
 *Get a random number between 0 and range    *
 *************************************************/
int Randomize(int range)
{
  
    return (int)((double)rand()/((double)RAND_MAX+1.0) * (double)(range+1));
}//end of Randomize()    
    
