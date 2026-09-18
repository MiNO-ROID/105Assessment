//
// Created by GGPC on 19/09/2026.
//

#ifndef INC_105ASSESSMENT_MANAGER_H
#define INC_105ASSESSMENT_MANAGER_H

#include <string>
class manager {
public:
    virtual void displayData() const = 0;
    virtual void loadData(const std::string& filename) = 0;
    virtual void saveData(const std::string& filename) const = 0;

    virtual ~manager() = default;
};


#endif //INC_105ASSESSMENT_MANAGER_H
