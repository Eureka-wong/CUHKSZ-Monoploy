#ifndef ACTION_WEIGHTS_H
#define ACTION_WEIGHTS_H
#include <vector>


class Weights{
public:
    Weights();

    void setWeights(const std::vector<double>& sellWeights,
                    const std::vector<double>& mortgageWeights,
                    const std::vector<double>& upgradeWeights,
                    const std::vector<double>& buyWeights);

    std::vector<double> getSellWeights() const;
    std::vector<double> getMortgageWeights() const;
    std::vector<double> getUpgradeWeights() const;
    std::vector<double> getBuyWeights() const;

private:

    std::vector<double> sellWeights;
    std::vector<double> mortgageWeights;
    std::vector<double> upgradeWeights;
    std::vector<double> buyWeights;

};

#endif // ACTION_WEIGHTS_H
