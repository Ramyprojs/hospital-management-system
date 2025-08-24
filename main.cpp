#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department {
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType {
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== PATIENT CLASS ========== //
class Patient {
private:
    int id;
    string name;
    int age;
    string contact;
    stack<string> medicalHistory;
    queue<string> testQueue;
    bool isAdmitted;
    RoomType roomType;

public:

    Patient(int pid, string n, int a, string c)
        : id(pid), name(n), age(a), contact(c), isAdmitted(false), roomType(GENERAL_WARD) {}


    void admitPatient(RoomType type) {
        if (!isAdmitted) {
            isAdmitted = true;
            roomType = type;
            string roomTypeStr;
            switch(type) {
                case GENERAL_WARD: roomTypeStr = "General Ward"; break;
                case ICU: roomTypeStr = "ICU"; break;
                case PRIVATE_ROOM: roomTypeStr = "Private Room"; break;
                case SEMI_PRIVATE: roomTypeStr = "Semi-Private"; break;
            }
            addMedicalRecord("Patient admitted to " + roomTypeStr);
        } else {
            addMedicalRecord("Admission attempted while already admitted");
        }
    }


    void dischargePatient() {
        if (isAdmitted) {
            isAdmitted = false;
            addMedicalRecord("Patient discharged");
        } else {
            addMedicalRecord("Discharge attempted while not admitted");
        }
    }


    void addMedicalRecord(string record) {
        medicalHistory.push(record);
    }


    void requestTest(string testName) {
        testQueue.push(testName);
        addMedicalRecord("Test requested: " + testName);
    }


    string performTest() {
        if (testQueue.empty()) {
            addMedicalRecord("No tests pending");
            return "No tests pending";
        }

        string testName = testQueue.front();
        testQueue.pop();
        addMedicalRecord("Test performed: " + testName);
        return testName;
    }


    void displayHistory() {
        if (medicalHistory.empty()) {
            cout << "No medical history available." << endl;
            return;
        }

        stack<string> tempStack;


        while (!medicalHistory.empty()) {
            tempStack.push(medicalHistory.top());
            medicalHistory.pop();
        }

        cout << "=== Medical History for Patient " << id << " ===" << endl;
        while (!tempStack.empty()) {
            cout << "- " << tempStack.top() << endl;
            medicalHistory.push(tempStack.top());
            tempStack.pop();
        }
        cout << "=================================" << endl;
    }


    int getId() { return id; }
    string getName() { return name; }
    bool getAdmissionStatus() { return isAdmitted; }
};

// ========== DOCTOR CLASS ========== //
class Doctor {
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;

public:
    Doctor(int did, string n, Department d){
        id = did;
        name = n;
        department = d;
    }

    void addAppointment(int patientId){
        appointmentQueue.push(patientId);
    }
    int seePatient(){
        if(appointmentQueue.size() == 0){
            cout << "Empty list." << endl;
            return -1;
        }
        int pid = appointmentQueue.front();
        appointmentQueue.pop();
        return pid;
    }

    int getId(){
        return id;
    }
    string getName(){
        return name;
    }
    string getDepartment(){
        switch (department)
        {
        case CARDIOLOGY:
            return "Cardiology";
        case NEUROLOGY:
            return "Neurology";
        case ORTHOPEDICS:
            return "Orthopedics";
        case PEDIATRICS:
            return "Pediatrics";
        case EMERGENCY:
            return "Emergency";
        case GENERAL:
            return "General";
        }
        return "Unknown";
    }
};

// ========== HOSPITAL CLASS ========== //
class Hospital {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    queue<int> emergencyQueue;
    int patientCounter;
    int doctorCounter;

public:
    Hospital() : patientCounter(1), doctorCounter(1) {}

    int registerPatient(string name, int age, string contact) {
        Patient newPatient(patientCounter, name, age, contact);
        patients.push_back(newPatient);
        int currentId = patientCounter;
        patientCounter++;
        cout << "Patient registered successfully with ID: " << currentId << endl;
        return currentId;
    }

    int addDoctor(string name, Department dept) {
        Doctor newDoctor(doctorCounter, name, dept);
        doctors.push_back(newDoctor);
        int currentId = doctorCounter;
        doctorCounter++;
        cout << "Doctor added successfully with ID: " << currentId << endl;
        return currentId;
    }

    void admitPatient(int patientId, RoomType type) {
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                patients[i].admitPatient(type);
                cout << "Patient " << patientId << " admission processed." << endl;
                return;
            }
        }
        cout << "Error: Patient with ID " << patientId << " not found." << endl;
    }

    void addEmergency(int patientId) {
        bool patientExists = false;
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                patientExists = true;
                break;
            }
        }

        if (patientExists) {
            emergencyQueue.push(patientId);
            cout << "Patient " << patientId << " added to emergency queue." << endl;
        } else {
            cout << "Error: Patient with ID " << patientId << " not found." << endl;
        }
    }

    int handleEmergency() {
        if (emergencyQueue.empty()) {
            cout << "No emergency cases pending." << endl;
            return -1;
        }

        int patientId = emergencyQueue.front();
        emergencyQueue.pop();
        cout << "Handling emergency for Patient " << patientId << endl;
        return patientId;
    }

    void bookAppointment(int doctorId, int patientId) {
        bool doctorFound = false;
        int doctorIndex = -1;
        for (int i = 0; i < doctors.size(); i++) {
            if (doctors[i].getId() == doctorId) {
                doctorFound = true;
                doctorIndex = i;
                break;
            }
        }

        bool patientFound = false;
        int patientIndex = -1;
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                patientFound = true;
                patientIndex = i;
                break;
            }
        }

        if (!doctorFound) {
            cout << "Error: Doctor with ID " << doctorId << " not found." << endl;
            return;
        }

        if (!patientFound) {
            cout << "Error: Patient with ID " << patientId << " not found." << endl;
            return;
        }

        doctors[doctorIndex].addAppointment(patientId);
        patients[patientIndex].addMedicalRecord("Appointment booked with Dr. " + doctors[doctorIndex].getName());
        cout << "Appointment booked: Patient " << patientId << " with Doctor " << doctorId << endl;
    }

    void displayPatientInfo(int patientId) {
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                cout << "\n=== PATIENT INFORMATION ===" << endl;
                cout << "ID: " << patients[i].getId() << endl;
                cout << "Name: " << patients[i].getName() << endl;
                cout << "Admission Status: " << (patients[i].getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
                patients[i].displayHistory();
                return;
            }
        }
        cout << "Error: Patient with ID " << patientId << " not found." << endl;
    }

    void displayDoctorInfo(int doctorId) {
        for (int i = 0; i < doctors.size(); i++) {
            if (doctors[i].getId() == doctorId) {
                cout << "\n=== DOCTOR INFORMATION ===" << endl;
                cout << "ID: " << doctors[i].getId() << endl;
                cout << "Name: " << doctors[i].getName() << endl;
                cout << "Department: " << doctors[i].getDepartment() << endl;
                cout << "=========================" << endl;
                return;
            }
        }
        cout << "Error: Doctor with ID " << doctorId << " not found." << endl;
    }
};

// ========== MAIN PROGRAM ========== //
int main() {
    Hospital hospital;

    cout << "=== HOSPITAL MANAGEMENT SYSTEM TEST ===" << endl << endl;

    // Test Case 1: Registering patients
    cout << "--- Test Case 1: Registering Patients ---" << endl;
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");
    cout << endl;

    // Test Case 2: Adding doctors
    cout << "--- Test Case 2: Adding Doctors ---" << endl;
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);
    cout << endl;

    // Test Case 3: Admitting patients
    cout << "--- Test Case 3: Admitting Patients ---" << endl;
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    hospital.admitPatient(p1, SEMI_PRIVATE);
    cout << endl;

    // Test Case 4: Booking appointments
    cout << "--- Test Case 4: Booking Appointments ---" << endl;
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    hospital.bookAppointment(999, p1); // Invalid doctor
    hospital.bookAppointment(d1, 999); // Invalid patient
    cout << endl;

    // Test Case 6: Emergency cases
    cout << "--- Test Case 6: Emergency Cases ---" << endl;
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency(); // No more emergencies
    cout << endl;

    // Test Case 8: Displaying information
    cout << "--- Test Case 8: Displaying Information ---" << endl;
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // Invalid patient

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // Invalid doctor
    cout << endl;

    // Test Case 10: Edge cases
    cout << "--- Test Case 10: Edge Cases ---" << endl;
    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1); // No patients
    emptyHospital.displayDoctorInfo(1);  // No doctors
    emptyHospital.handleEmergency();     // No emergencies

    return 0;
}
