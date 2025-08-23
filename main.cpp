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
    Hospital();
    
    int registerPatient(string name, int age, string contact);
    int addDoctor(string name, Department dept);
    void admitPatient(int patientId, RoomType type);
    void addEmergency(int patientId);
    int handleEmergency();
    void bookAppointment(int doctorId, int patientId);
    void displayPatientInfo(int patientId);
    void displayDoctorInfo(int doctorId);
};

// ========== MAIN PROGRAM ========== //
int main() {
    Hospital hospital;
    
    // Test Case 1: Registering patients
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");
    
    // Test Case 2: Adding doctors
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);
    
    // Test Case 3: Admitting patients
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    // Try admitting already admitted patient
    hospital.admitPatient(p1, SEMI_PRIVATE);
    
    // Test Case 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    // Try booking with invalid doctor/patient
    hospital.bookAppointment(999, p1); // Invalid doctor
    hospital.bookAppointment(d1, 999); // Invalid patient
    
    // Test Case 5: Handling medical tests
    // These would normally be called on Patient objects
    // In a real implementation, we'd need a way to access patients
    
    // Test Case 6: Emergency cases
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency(); // No more emergencies
    
    // Test Case 7: Discharging patients
    // Would normally call dischargePatient() on Patient objects
    
    // Test Case 8: Displaying information
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // Invalid patient
    
    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // Invalid doctor
    
    // Test Case 9: Doctor seeing patients
    // These would normally be called on Doctor objects
    // In a real implementation, we'd need a way to access doctors
    
    // Test Case 10: Edge cases
    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1); // No patients
    emptyHospital.displayDoctorInfo(1);  // No doctors
    emptyHospital.handleEmergency();     // No emergencies
    
    return 0;
}