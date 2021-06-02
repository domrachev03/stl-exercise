#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <string>
#include <list>
#include <climits>

using namespace std;

//ITEM 3.f: the following three classes (shuffle, merge sort and printing) use pointers to a data structure
// as it allows to generalize this algorithms to an extend

/*
 * Function mergeSortIterators
 *  Sort the sequence of pointers between begin and end iterators, applying merge sort algo
 *
 *  Template parameters:
 *      class Iterator - class of the Iterator. This iterator has to be bidirectional
 *
 *  Function parameters:
 *      const Iterator& begin - iterator, which points on the begin of the interval
 *      const Iterator& end - iterator, which points on the end of the interval
 */
template<class Iterator>
void mergeSortIterators(const Iterator& begin, const Iterator& end) {
    //Calculating the length of the interval
    int interval_length = 0;
    for(auto i = begin; i != end; i++) {
        interval_length++;
    }
    // If the length of the current interval is less than 2, do nothing
    if(interval_length <= 1) {
        return;
    }

    Iterator middle = begin;
    for(int i = 0; i < interval_length / 2; i++) {
        middle++;
    }

    // Splitting the interval on two subsequences, and sorting them separately
    mergeSortIterators(begin, middle);
    mergeSortIterators(middle, end);

    // Vector, which contains the result of merge process
    vector<decay_t<decltype(*begin)>> intermediate_vector;
    // Iterators, pointing on the beginning of two sorted subsequences
    Iterator first_index = begin, second_index = middle;

    // Merging process
    while(first_index != middle && second_index != end) {
        if(*first_index > *second_index) {
            intermediate_vector.push_back(*first_index);
            first_index++;
        }
        else {
            intermediate_vector.push_back(*second_index);
            second_index++;
        }
    }

    while(first_index != middle) {
        intermediate_vector.push_back(*first_index);
        first_index++;
    }
    while(second_index != end) {
        intermediate_vector.push_back(*second_index);
        second_index++;
    }

    // Copying merge process result to a initial sequence
    auto iter = begin;
    for(auto it = intermediate_vector.begin(); it != intermediate_vector.end(); it++, iter++) {
        *iter = *it;
    }
}

/*
 * Function shuffle
 *  Shuffles the sequence, by making length*scale_factor random permutations
 *
 *  Template parameters:
 *      class Iterator - class of the Iterator. This iterator has to be bidirectional
 *
 *  Function parameters:
 *      const Iterator& begin - iterator, which points on the begin of the interval
 *      const Iterator& end - iterator, which points on the end of the interval
 */

template<class Iterator>
void shuffle(const Iterator& begin, const Iterator& end) {
    // Constant, which denotes the amount of permutations
    constexpr int scale_factor = 5;

    // Calculating the length of the interval
    int length = 0;
    for(auto i = begin; i != end; i++) {
        length++;
    }
    const int permutations_amount = length * scale_factor;

    // Setting random seed
    srand(time(0));

    for(int i = 0; i < permutations_amount; ++i) {
        auto p1 = begin;
        auto p2 = begin;

        // Randomly choosing two elements to permute
        int p1_pos = rand() % length, p2_pos = rand() % length;

        // Finding the position of two chosen elements
        for(int k = 0; k < max(p1_pos, p2_pos); ++k) {
            if(k < p1_pos) {
                p1++;
            }
            if(k < p2_pos) {
                p2++;
            }
        }

        auto tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
    }
}

/*
 * Function printPointers
 *  Printing the given sequence of pointers
 *
 *  Template parameters:
 *      class Iterator - class of the Iterator. This iterator has to be bidirectional
 *
 *  Function parameters:
 *      const Iterator& begin - iterator, which points on the begin of the interval
 *      const Iterator& end - iterator, which points on the end of the interval
 */

template<class Iterator>
void printPointers(const Iterator& begin, const Iterator& end) {
    for(auto it = begin; it != end; it++) {
        // Method string has to be overloaded for the given value
        // For my implementation, every class which inherits Thing class
        // Has to have string() method
        cout << (string)*(*it) << "\n";
    }
    cout << "---------------------------------------------------\n";
}

//ITEM 3.g: here, a predication function is used, as this allows to generalize this function
template<typename T>
void printVectorConditionally(vector<T>& v, bool (*f)(T&)) {
    for(int i = 0; i < v.size(); ++i) {
        // Checking the value, by passing it to the predicate function.
        // Method string has to be overloaded for the given value
        // For my implementation, every class which inherits Thing class
        // Has to have string() method
        if(f(v[i])) {
            cout << (string)* v[i] << "\n";
        }
    }
    cout << "---------------------------------------------------\n";
}

class Thing {

public:
    //Static variable, which contains current last id available
    static int last_id;

    Thing() {
        //Giving to the new thing id
        id = last_id;
        last_id++;
    }
    //Copy constructor. I define it, as it is convenient to have an opportunity to create a clone
    Thing(Thing& other_thing) {
        //Giving to the new thing id
        id = other_thing.id;
    }
    //Explicitly set destructor as a virtual default class.
    //Otherwise, destruction of any child of Thing through base class
    //will be undefined
    virtual ~Thing() = default;

    //denoting pure virtual function - conversion to string
    //All children of this class have to have conversion to the string
    virtual explicit operator string() = 0;

    //Returning id of the element
    int get_id() const {
        return id;
    }

    //Returning string, which contains the id of the element
    string get_string_id() const {
        return ::to_string(id);
    }

    //Overloading all things comparisons as comparisons of special criteria
    virtual strong_ordering operator<=> (const Thing& other_thing) const {
        return comparision_criteria <=> other_thing.comparision_criteria;
    }

protected:
    //This function return criteria of the class.
    //This function is virtual, as it has to be implemented in every child class.
    virtual int get_criteria() = 0;

    //Special criteria, which allows to compare different classes between each other
    int comparision_criteria;

private:
    //id of this thing
    int id;
};
//Initializing static variable right after declaration of the class
int Thing::last_id = 0;


/*
 * Class Car
 *      Class, which represents a car object.
 *      Car inherits Thing virtually in order to avoid directed acyclic graph.
 */
class Car: public virtual Thing {
public:
    //Constructor, which initialize all the field of the class
    explicit Car(const string& colour): Thing() {
        this->colour = colour;
        comparision_criteria = get_criteria();
    }

    //Default constructor, which initialize car with some basic colour
    Car(): Car("white") {}

    //Getter for field colour
    string get_colour() const {
        return colour;
    }

    string& get_colour_reference() {
        return colour;
    }

    //Defining cast to string
    explicit operator string() override{
        return "car with id " + get_string_id() + " colour " + colour;
    }

    //Defining the assignment
    Car& operator=(const Car& other_car) {
        colour = other_car.colour;
        return *this;
    }

protected:
    //Defining criteria value
    int get_criteria() override {
        return 8;
    }

private:
    string colour;
};

/*
 * Enum genders
 *      Contains all possible genders
 */
enum Genders {
    FEMALE,
    MALE
};

/*
 * class Person
 *      Class, which represents a person.
 *      Person inherits Thing virtually in order to avoid directed acyclic graph.
 */
class Person: public virtual Thing { //virtual class
public:
    template<typename T>
    Person(T sex) = delete;

    //Constructor, which initializes all field of the class
    explicit Person(Genders sex): Thing() {
        gender = sex;
        comparision_criteria = get_criteria();
    }

    //Default constructor, which initialises person with default parameters
    Person(): Person(MALE){}

    //getter for gender, which returns enum value
    Genders get_gender() {
        return gender;
    }

    //getter for gender, which returns string value
    string get_gender_string(){
        switch(gender) {
            case MALE:
                return "male";
            case FEMALE:
                return "female";
            default:
                return "undefined";
        }
    }

    //Defining cast to string
    explicit operator string() override{
        return "person with id " + get_string_id() + ", sex " + get_gender_string();
    }

    //Defining assignment as assignment of gender
    Person& operator= (const Person& other_person) {
        gender = other_person.gender;
        return *this;
    }

protected:
    //In this class, comparison criteria is a gender: 0, 1, ...
    int get_criteria() override {
        return gender;
    }

private:
    Genders gender;
};

/*
 * class Student
 *      Class, which represents a student.
 */
class Student: public Person {
public:
    //Constructor, which initialises all fields of the class
    explicit Student(const Genders& sex, const string& name): Person(sex) {
        this->name = name;
        comparision_criteria = get_criteria();
    }

    //Default constructor, which initialises fields by default parameters
    Student(): Student(MALE, "Ivan"){};

    //getter for the name
    string get_name() {
        return name;
    }

    //Defining cast to string
    explicit operator string() override {
        return Person::operator string() + " and name " + name;
    }

    //Defining assignment as assignment of name and assignment of the base class
    Student& operator=(const Student& other_student) {
        Person::operator=(other_student);
        name = other_student.name;
        return *this;
    }

protected:
    //Defining criteria as comparison criteria of the base class plus constant
    int get_criteria() override{
        return 4 + Person::comparision_criteria;
    }
private:
    string name;
};

class Transformer: public Car, public Person {
public:
    //Constructor, which initialises all fields of the class
    Transformer(const Genders& sex, const string& colour, const int& power): Car(colour), Person(sex) {
        this->power = power;
        comparision_criteria = get_criteria();
    }

    //Default constructor, which initialises fields by default values
    explicit Transformer(): Transformer(MALE, "white", 5) {}

    //Defining conversion to string
    explicit operator string() override {
        return "transformer with id " + Thing::get_string_id() + ", sex " + Person::get_gender_string() +
               ", power " + to_string(power) + " and a color " + Car::get_colour();
    }

    //getter for the power
    int get_power() const{
        return power;
    }

    //defining assignment as assignment of the fields
    Transformer& operator= (const Transformer& other_transformer) {
        Car::operator=(other_transformer);
        Person::operator=(other_transformer);
        power = other_transformer.power;
        return *this;
    }

protected:
    //Criteria is calculated as criteria of the two base classes plus constant
    int get_criteria() override{
        return Car::get_criteria() + Person::get_criteria() - 2;
    }

private:
    int power;
};

//Special formula for computing power of transformers
constexpr int calculate_power(int power) {
    int ans = 1;
    for(int i = 0; i < power; ++i) {
        ans*=2;
    }
    return ans;
}

//ITEM 3.b: Non-type parameter is used because wrapper has to get an arbitrary function which it will wrap up.

/*
 * Class Wrapper
 *  Special wrapper for arbitrary data structure, which can apply functions to given data structure
 *
 *  Template parameters:
 *      class DataStrut - type of data structure
 */
template<class DataStruct>
class Wrapper{
public:

    /*
     * Method apply_function
     *  Applies given function to a given subsequence of data structure.
     *  By default, applied to the whole sequence
     *
     *  Template parameters:
     *      class Function - type of operation, which will be applied
     *
     *  Method parameters:
     *      DataStruct& data - data structure, for which operation will be applied
     *      const Function& f - instance of the function, which will be applied to data structure
     *      int from = 0 - begin of subsequence
     *      int to = INT_MAX - end of subsequence. If to not defined, define it as length of the data structure
     */
    template<class Function>
    void apply_function(DataStruct& data, const Function& f, int from = 0, int to = INT_MAX) {
        if(to == INT_MAX) {
            to = data.size();
        }
        using DataType = typename DataStruct::value_type;
        // Translating stack to a vector
        vector<DataType> copy_vector;
        for(auto i = data.begin(); i != data.end(); i++) {
            copy_vector.push_back(*i);
        }

        // Making pointers for begin and end
        auto from_iterator = copy_vector.begin() + from;
        auto to_iterator = copy_vector.begin() + to;

        // Applying function to a vector, representing stack
        f(from_iterator, to_iterator);

        // Putting elements of vector back to stack
        auto init_it = data.begin();
        for(auto copy_it = from_iterator; copy_it != to_iterator; copy_it++, init_it++) {
            *init_it = *copy_it;
        }
    }
};

//ITEM 3.d: wrapper for stack is special, because it is translated to the vector and back not with pointers,
// but with popping/adding elements

//Overload of the function for a stack
template<class T>
class Wrapper<stack<T>>{
public:
    template<class Function>
    void apply_function(stack<T>& data, const Function& f, int from = 0, int to = INT_MAX) {
        if(to == INT_MAX) {
            to = data.size();
        }
        // Translating stack to a vector
        vector<T> stack_vector;

        //stack don't have pointers, so popping all elements form it and puting them in vector
        while(!data.empty()) {
            stack_vector.push_back(data.top());
            data.pop();
        }
        reverse(stack_vector.begin(), stack_vector.end());

        // Making pointers for begin and end
        auto from_iterator = stack_vector.begin() + from;
        auto to_iterator = stack_vector.begin() + to;

        // Applying function to a vector, representing stack
        f(from_iterator, to_iterator);

        // Putting elements of vector back to stack
        for(auto it = stack_vector.begin(); it != stack_vector.end(); it++) {
            data.push(*it);
        }
    }
};

// Special predicate function, which checks if the given Thing is a woman or not
bool womanPower(Thing *&v) {
    // If object can't be casted, return nullptr
    auto cast_to_person = dynamic_cast<Person*>(v);
    if(cast_to_person && cast_to_person->get_gender_string() == "female") {
        return true;
    }
    return false;
}

int main() {
    vector<Thing*> things;


    constexpr int black_power = calculate_power(7);
    constexpr int yellow_power = calculate_power(10);
    constexpr int blue_power = calculate_power(15);

    things.push_back(new Transformer(FEMALE, "Black", black_power));
    things.push_back(new Student(FEMALE, "Anna"));
    things.push_back(new Student());
    things.push_back(new Transformer(MALE, "Blue", blue_power));
    things.push_back(new Student(MALE, "Dima"));
    things.push_back(new Student(FEMALE, "Margarita"));
    things.push_back(new Car("Green"));
    things.push_back(new Student(FEMALE, "Kopenina"));
    things.push_back(new Transformer(FEMALE, "Black", yellow_power));
    things.push_back(new Student(MALE, "Ivan"));
    things.push_back(new Car("Red"));
    things.push_back(new Student(MALE, "Selim"));

    //ITEM 3.a: the usage of implemented classes/functions is below
    list<Thing*> things_list;
    stack<Thing*> things_stack;
    for(auto element: things) {
        things_stack.push(element);
        things_list.push_back(element);
    }
    cout << "FOR VECTOR: \n";
    printPointers(things.begin(), things.end());
    shuffle(things.begin(), things.end());
    printPointers(things.begin(), things.end());
    mergeSortIterators(things.begin(), things.end());
    printPointers(things.begin(), things.end());
    printPointers(things.end()-1, things.end());

    //ITEM 3.c: functions printPointers, shuffle and printPointers are called for vector and list
    // As this algorithms work with bidirectional pointers
    cout << "\nFOR LIST: \n";
    printPointers(things_list.begin(), things_list.end());
    shuffle(things_list.begin(), things_list.end());
    printPointers(things_list.begin(), things_list.end());
    mergeSortIterators(things_list.begin(), things_list.end());
    printPointers(things_list.begin(), things_list.end());
    printPointers(--things_list.end(), things_list.end());

    cout << "\nFOR LIST WITH CONTAINER: \n";
    //ITEM 3.e: the decltype here helps to obtain the iterator class
    Wrapper<list<Thing*>> s;
    s.apply_function<decltype(printPointers<vector<Thing*>::iterator>)>(things_list, printPointers);
    s.apply_function<decltype(shuffle<vector<Thing*>::iterator>)>(things_list, shuffle);
    s.apply_function<decltype(printPointers<vector<Thing*>::iterator>)>(things_list, printPointers);
    s.apply_function<decltype(mergeSortIterators<vector<Thing *>::iterator>)>(things_list, mergeSortIterators);
    s.apply_function<decltype(printPointers<vector<Thing*>::iterator>)>(things_list, printPointers);
    s.apply_function<decltype(printPointers<vector<Thing*>::iterator>)>
            (things_list, printPointers, 0, 1);

    cout << "\nFOR STACK: \n";
    Wrapper<stack<Thing*>> stack_wrapper;
    stack_wrapper.apply_function<decltype(printPointers<vector<Thing*>::iterator>)>(things_stack, printPointers);
    stack_wrapper.apply_function<decltype(shuffle<vector<Thing*>::iterator>)>(things_stack, shuffle);
    stack_wrapper.apply_function<decltype(printPointers<vector<Thing*>::iterator>)>(things_stack, printPointers);
    stack_wrapper.apply_function<decltype(mergeSortIterators<vector<Thing *>::iterator>)>
            (things_stack, mergeSortIterators);
    stack_wrapper.apply_function<decltype(printPointers<vector<Thing*>::iterator>)>(things_stack, printPointers);
    stack_wrapper.apply_function<decltype(printPointers<vector<Thing*>::iterator>)>
            (things_stack, printPointers, things_stack.size()-1, things_stack.size());

    cout << "\nWOMAN POWER\n";
    printVectorConditionally(things, &womanPower);
}