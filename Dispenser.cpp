/**
 * @file Dispenser.cpp
 * @author Charles Pieczonka, Marissa Liu, Fletcher Smith, Maya Murad, Julie Winer
 * @date 28 Nov 2019
 * @brief Implementation of our dispenser class
 *
 * This dispenser implementation contains methods that add and remove
 * individual hoppers to the dispenser, as well as methods that index
 * the current hopper and open and close the correct hoppers
 */

#include "hopper.hpp"
#include "dispenser.hpp"
#include "wiringPi.h"
#include "softServo.h"
using namespace std;

/**
 * @brief Dispenser constructor
 *
 * Initializes an instance of the dispenser class and starts
 * the current index begins at zero as well as the size (amount of hoppers)
 */
Dispenser::Dispenser() {
    this->current_index = 0;
    this->size = 0;
}

/**
 * @brief Dispenser destructor
 *
 * Any memory that needs to be freed will be done here
 */
Dispenser::~Dispenser() {
}

/**
 * @brief Gets the current candy hopper in the sequence.
 *
 * The current index is used for a variety of reasons, including
 * to determine what servo motor to activate when dispensing.
 * Since the current_index variable is private a getter is needed.
 *
 * @returns The id of the current candy hopper
 */
int Dispenser::getCurrentIndex() {
    return this->current_index;
}

/**
 * @brief Gets the size of the dispenser
 *
 * The size of the dispenser refers to the amount of hoppers associated
 * with the dispenser class. Since size is a private variable, a getter is needed.
 *
 * @returns The number of hoppers in the specified Dispenser
 */
int Dispenser::getSize() {
    return this->size;
}

/**
 * @brief Gets an instance of the hopper class at the specified index
 *
 * If the new index provided is out of range, the Dispenser will select
 * the position at new_index modulus the number of hoppers in our system.
 *
 * @param index The index of the hopper to be retrieved
 * @returns A pointer to the hopper at the given index
 * @throws An Out of Range exception if there are no hoppers to select
 */
Hopper* Dispenser::getHopper(int index) {
    if (this->size == 0) {
        throw out_of_range("Error: No hoppers available.");
    }
    return this->hoppers[index % this->size];
}

/**
 * @brief Gets an instance of the hopper class at the current index
 *
 * An overload of the getHopper method that returns the hopper at the current index
 * if no index is specified as a parameter
 *
 * @returns A pointer to the hopper at the current index
 * @throws An Out of Range exception if there are no hoppers to select
 */
Hopper* Dispenser::getHopper() {
    if (this->size == 0) {
        throw out_of_range("Error: No hoppers available.");
    }
    return this->getHopper(this->current_index);
}

/**
 * @brief Changes the index of the current candy hopper
 *
 * If the new index provided is out of range, the Dispenser will select
 * the position at new_index modulus the number of hoppers in our system.
 *
 * @param new_index The index value to be assigned to the hopper
 * @return The id of the new candy hopper
 * @throws An Out of Range exception if there are no hoppers to select
 */
int Dispenser::setCurrentIndex(int new_index) {
    if (this->size == 0) {
        throw out_of_range("Error: No hoppers available.");
    }
    this->current_index = new_index % this->size;
    return this->current_index;
}

/**
 * @brief Selects the next candy hopper in the sequence.
 *
 * This method is used to index which hopper is selected
 * and this change the LED that is lit, as well as the servo that will
 * be activated upon dispensing. The method simply changes the current index
 * to the next hopper in line.
 *
 * @returns The id of the next candy hopper
 * @throws An Out of Range exception if there are no hoppers to select
 */
int Dispenser::nextIndex() {
    if (this->size == 0) {
        throw out_of_range("Error: No hoppers available.");
    }
    this->current_index = (this->current_index + 1) % this->size;
    
    return this->current_index;
}

/**
 * @brief Open the hopper at the current index
 *
 * This method utilizes current index to power the associated servo motor
 * Thus opening the hopper and dispensing the candy. If the current index is
 * on the multicoloured LED, all hoppers are opened.
 *
 * @throws An Out of Range exception if there are no hoppers to select
 */
void Dispenser::openDispenser() {
    if (this->size == 0) {
        throw out_of_range("Error: No hoppers available.");
    }
    
    if (current_index == 4){
        //if indexed on our multicoloured light, open all hoppers
        for (int i = 0; i < this->size; i++){
            softServoWrite(i, 60);
        }
    }
    else{
        
        // use current index's servo to open hopper
        softServoWrite((current_index+5), 60);
    }
}


/**
 * @brief Close the hopper at the current index
 *
 * This method utilizes current index to power the associated servo motor
 * Thus closing the hopper. If the current index is on the multicoloured
 * LED, all hoppers are closed.
 *
 * @throws An Out of Range exception if there are no hoppers to select
 */
void Dispenser::closeDispenser() {
    if (this->size == 0) {
        throw out_of_range("Error: No hoppers available.");
    }
    
    if (current_index == 4){
        //if indexed on our multicoloured light, close all hoppers
        for (int i = 0; i < this->size; i++){
            softServoWrite(i, 0);
        }
    }
    else{
        
        // use current index's servo to open hopper
        softServoWrite((current_index+5), 0);
    }
}

/**
 * @brief Appends a pointer to a Hopper object at the end of the hoppers vector.
 *
 * This method is an overload of addHopper that adds an instance of the hopepr class
 * to the end of the hopper vector rather than at a specified index.
 *
 * @param new_hopper An instance of the hopper class to be added to the dispenser
 * @returns The index of the new hopper pointer
 * @throws An Out of Range exception if there are no hoppers to select
 */
int Dispenser::addHopper(Hopper* new_hopper) {
    return this->addHopper(new_hopper, this->size);
}

/**
 * @brief Adds a pointer to a Hopper object at the specified index
 *
 * If an index is provided, the new hopper will be added to the position at new_index
 * modulus the number of hoppers in our system. Otherwise, the new hopper is appended to the end.
 *
 * @param new_hopper An instance of the hopper class to be added to the dispenser
 * @param index The index we want to assign to the hopper
 * @returns The index of the new hopper pointer
 * @throws an Out of Range exception if there are no hoppers to select
 */
int Dispenser::addHopper(Hopper* new_hopper, int index) {
    if (this->size == 0) {
        throw out_of_range("Error: No hoppers available.");
    }
    if (index == this->size) {
        hoppers.push_back(new_hopper);
        this->size++;
    } else {
        this->size++;
        int insert_index = index % this->size;
        hoppers.insert(hoppers.begin() + insert_index, new_hopper);
    }
    return index % this->size;
}

/**
 * @brief Removes the hopper at a specified index in the hopper vector.
 *
 * The hopper at the position at index modulus the number of hoppers in our system will be removed.
 *
 * @param index The index of the hopper to be removed
 */
void Dispenser::removeHopper(int index) {
    this->hoppers.erase(this->hoppers.begin() + (index % this->size));
}

/**
 * @brief Removes a the specified hopper from the dispenser
 *
 * Removes a hopper based on the hopper class instance itself rather than
 * the index of the hopper in the hopper vector
 *
 * @param hopper The hopper class instance to be removed
 */
void Dispenser::removeHopper(Hopper* hopper) {
    vector<Hopper*>::iterator it = std::find(this->hoppers.begin(), this->hoppers.end(), hopper);
    int position = distance(this->hoppers.begin(), it);
    this->removeHopper(position);
}


