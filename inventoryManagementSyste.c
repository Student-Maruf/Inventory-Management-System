#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store product details
struct Product {
    int id;           // Product ID
    char name[50];    // Product name
    int quantity;     // Product quantity
    float price;      // Product price
};

// Function declarations
void addProduct();
void displayProducts();
void searchProduct();
void updateProduct();
void deleteProduct();

int main() {
    int choice;

    // Infinite loop for menu until user chooses to exit
    while (1) {
        printf("\n====== Inventory Management System ======\n");
        printf("1. Add Product\n");
        printf("2. Display Products\n");
        printf("3. Search Product\n");
        printf("4. Update Product\n");
        printf("5. Delete Product\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        // Perform action based on user's choice
        switch (choice) {
            case 1: addProduct(); break;
            case 2: displayProducts(); break;
            case 3: searchProduct(); break;
            case 4: updateProduct(); break;
            case 5: deleteProduct(); break;
            case 0: exit(0); // Exit program
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}

// Function to add a new product
void addProduct() {
    struct Product p;
    FILE *fp = fopen("inventory.dat", "ab"); // Open file in append mode

    // Input product details
    printf("Enter ID: ");
    scanf("%d", &p.id);
    printf("Enter Name: ");
    scanf(" %[^\n]", p.name); // Read string with spaces
    printf("Enter Quantity: ");
    scanf("%d", &p.quantity);
    printf("Enter Price: ");
    scanf("%f", &p.price);

    // Write product to file
    fwrite(&p, sizeof(p), 1, fp);
    fclose(fp);

    printf("Product added successfully!\n");
}

// Function to display all products
void displayProducts() {
    struct Product p;
    FILE *fp = fopen("inventory.dat", "rb"); // Open file in read mode

    if (!fp) {
        printf("No data found!\n");
        return;
    }

    printf("\nID\tName\t\tQty\tPrice\n");
    printf("--------------------------------------\n");

    // Read and display each product from file
    while (fread(&p, sizeof(p), 1, fp)) {
        printf("%d\t%-15s\t%d\t%.2f\n", p.id, p.name, p.quantity, p.price);
    }
    fclose(fp);
}

// Function to search for a product by ID
void searchProduct() {
    int id, found = 0;
    struct Product p;
    FILE *fp = fopen("inventory.dat", "rb");

    printf("Enter product ID to search: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.id == id) {
            printf("\nFound: %d %s %d %.2f\n", p.id, p.name, p.quantity, p.price);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
        printf("Product not found!\n");
}

// Function to update a product's quantity and price
void updateProduct() {
    int id, found = 0;
    struct Product p;
    FILE *fp = fopen("inventory.dat", "rb+"); // Open for reading and writing

    printf("Enter product ID to update: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.id == id) {
            // Get new details
            printf("Enter new quantity: ");
            scanf("%d", &p.quantity);
            printf("Enter new price: ");
            scanf("%f", &p.price);

            // Move file pointer back and update record
            fseek(fp, -sizeof(p), SEEK_CUR);
            fwrite(&p, sizeof(p), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found)
        printf("Product updated successfully!\n");
    else
        printf("Product not found!\n");
}

// Function to delete a product by ID
void deleteProduct() {
    int id;
    struct Product p;
    FILE *fp = fopen("inventory.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb"); // Temporary file

    printf("Enter product ID to delete: ");
    scanf("%d", &id);

    // Copy all products except the one to delete
    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.id != id)
            fwrite(&p, sizeof(p), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    // Replace old file with updated file
    remove("inventory.dat");
    rename("temp.dat", "inventory.dat");

    printf("Product deleted successfully!\n");
}
