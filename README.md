# Task 1: Data Parsing and Memory Management

1. **trim**  
   - Removes leading and trailing whitespace from strings.    
   - Called throughout the rest of the implementation.

2. **read_secretariat**  
   - Initializes all fields ("students", "subjects", "enrollments") to NULL.  
   - Reads the input file line by line, detecting [STUDENTS], [SUBJECTS] and [ENROLLMENTS] sections.  
   - For each section:  
     - Parses the current line.  
     - Dynamically allocates memory for the corresponding entity.  
     - Appends it to the secretariat structure.  
     - Validates format and data to prevent memory corruption or failed allocations.  
   - After parsing:  
     - Calculates each student’s overall average by iterating through their enrollment records.  
     - Closes the file and returns the populated structure.  
     - Calls "trim" to guarantee all strings are clean.

3. **add_student**  
   - Dynamically appends a new student to the existing data structure.  
   - Updates the total student count.  
   - Allocates memory for the new entry.  
   - Essential for extending the student list in future features.

4. **free_secretariat**  
   - Frees all memory allocated by read_secretariat.  
   - Prevents memory leaks, especially with complex allocations.

---

# Task 3: Block‐Cipher Encryption

1. **"IV" Preparation**  
   - Extend the Initialization Vector to match the cipher’s block size.

2. **Block Processing**  
   For each block of plaintext:  
   - XOR with the provided key.  
   - Apply P‐Box transformation (bit‐level diffusion).  
   - Apply S‐Box substitution (nonlinear mixing).

3. **Output**  
   - Ensures each ciphertext block depends on its plaintext and all previous blocks.  
   - Writes the final encrypted data to a compact binary file.