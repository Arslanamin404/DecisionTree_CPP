#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <iomanip>

using namespace std;

class DecisionTree
{
private:
    // vector<int> student = {1, 2, 3, 4, 5};
    // vector<string> outlook = {"sunny", "sunny", "overcast", "rain", "rain"};
    // vector<string> windspeed = {"weak", "strong", "weak", "weak", "strong"};
    // vector<string> playTennis = {"no", "no", "yes", "yes", "no"};
    map<string, vector<string>> features;
    vector<string> target;
    map<string, double> featureInfoGain;

    int yes_count = 0;
    int no_count = 0;
    void set_yes_no_count()
    {
        yes_count = 0, no_count = 0;
        // for (int i = 0; i < playTennis.size(); i++)
        //     (playTennis[i] == "yes") ? yes_count++ : no_count++;
        for (const auto &val : target)
            (val == "yes") ? yes_count++ : no_count++;
    }

    inline double get_probablity_of_yes() { return (double)yes_count / target.size(); }
    inline double get_probablity_of_no() { return (double)no_count / target.size(); }

    double get_table_entropy()
    {
        double p_yes = get_probablity_of_yes();
        double p_no = get_probablity_of_no();
        return entropy(p_yes) + entropy(p_no);
    }

    double calculate_feature_entropy(const vector<string> &feature)
    {
        set<string> unique_values(feature.begin(), feature.end());
        double total_weighted_entropy = 0.0;

        for (const auto &val : unique_values)
        {
            int yes_subset = 0, no_subset = 0, total_subset = 0;

            for (int i = 0; i < (int)feature.size(); i++)
            {
                if (feature[i] == val)
                {
                    total_subset++;
                    if (target[i] == "yes")
                        yes_subset++;
                    else
                        no_subset++;
                }
            }

            double p_yes = (total_subset == 0) ? 0.0 : (double)yes_subset / (double)total_subset;
            double p_no = (total_subset == 0) ? 0.0 : (double)no_subset / (double)total_subset;

            double subset_entropy = entropy(p_yes) + entropy(p_no);

            cout << "Value: " << val
                 << " | Total: " << total_subset
                 << " | Yes: " << yes_subset
                 << " | No: " << no_subset
                 << " | Subset entropy: " << subset_entropy << endl;

            total_weighted_entropy += ((double)total_subset / (double)feature.size()) * subset_entropy;
        }
        return total_weighted_entropy;
    }

    /*
    *TODO:
        CREATE A METHOD, THAT WILL SET THE DYNAMIC DATASET OF mXn dimention.
        we can group the above dataset as features and target. using map dataset
    */

public:
    DecisionTree() {}
    void set_dataset(const map<string, vector<string>> &features, const vector<string> &target)
    {
        this->features = features;
        this->target = target;
        if (!target.empty())
        {
            set_yes_no_count();
            calculate_info_gain();
        }
    }

    void display_dataset()
    {
        // cout << "--------------------------------------------------------------------------------" << endl;
        // cout << "STUDENT_ID" << setw(18) << "OUTLOOK" << setw(22) << "WINDSPEED" << setw(22) << "PLAY_TENNIS" << endl;
        // cout << "--------------------------------------------------------------------------------" << endl;
        // for (int i = 0; i < student.size(); i++)
        //     cout << setw(5) << student[i]
        //          << setw(22) << outlook[i]
        //          << setw(22) << windspeed[i]
        //          << setw(22) << playTennis[i] << endl;

        // cout << "--------------------------------------------------------------------------------" << endl;

        cout << "--------------------------------------------------------------------------------" << endl;
        cout << setw(12) << "RowID";
        for (const auto &f : features)
            cout << setw(15) << f.first;
        cout << setw(15) << "Target" << endl;
        cout << "--------------------------------------------------------------------------------" << endl;

        for (int i = 0; i < target.size(); i++)
        {
            cout << setw(12) << (i + 1);
            for (const auto &f : features)
                cout << setw(15) << f.second[i];
            cout << setw(15) << target[i] << endl;
        }
        cout << "--------------------------------------------------------------------------------" << endl;
    }

    double entropy(double p) { return (p == 0) ? 0 : -p * log2(p); }
    void calculate_info_gain()
    {
        featureInfoGain.clear();
        double table_ent = get_table_entropy();
        cout << "TABLE ENTROPY: " << table_ent << endl
             << endl;

        for (const auto &entry : features)
        {
            string f_name = entry.first;
            const vector<string> &f_values = entry.second;

            cout << "=== " << f_name << " ===" << endl;
            double f_entropy = calculate_feature_entropy(f_values);
            double f_info_gain = table_ent - f_entropy;

            cout << "Weighted entropy (" << f_name << ") = " << f_entropy << endl;
            cout << "Information gain (" << f_name << ") = " << f_info_gain << endl
                 << endl;

            featureInfoGain[f_name] = f_info_gain;
        }
    }

    void decide_root()
    {
        string bestFeature;
        double maxGain = -1.0;

        cout << "\n=== INFORMATION GAIN SUMMARY ===" << endl;
        for (const auto &entry : featureInfoGain)
        {
            cout << "Feature: " << setw(12) << entry.first
                 << " | InfoGain: " << entry.second << endl;

            if (entry.second > maxGain)
            {
                maxGain = entry.second;
                bestFeature = entry.first;
            }
        }

        cout << "\nRoot Node: " << bestFeature
             << " (InfoGain = " << maxGain << ")" << endl;
    }
};

int main()
{
    DecisionTree dt;

    map<string, vector<string>> features = {
        {"outlook", {"sunny", "sunny", "overcast", "rain", "rain"}},
        {"windspeed", {"weak", "strong", "weak", "weak", "strong"}},
        {"temp", {"high", "high", "mild", "cool", "mild"}}};
    vector<string> target = {"yes", "no", "yes", "yes", "no"};
    dt.set_dataset(features, target);
    dt.display_dataset();
    cout << "\n=======================================================================" << endl;
    dt.decide_root();
    cout << "\n=======================================================================" << endl;
    return 0;
}