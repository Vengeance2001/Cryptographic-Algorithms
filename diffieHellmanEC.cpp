#include <iostream>
#include <utility>

using namespace std;

// Elliptic Curve Cryptography (ECC)
class EllipticCurve {
private:
    long long a, b; // Curve parameters
public:
    EllipticCurve(long long param_a, long long param_b) : a(param_a), b(param_b) {}

    pair<long long, long long> scalarMultiply(long long scalar, long long baseX, long long baseY) {
        long long x = baseX, y = baseY;
        for (long long i = 1; i < scalar; ++i) {
            // Perform point addition (insecure for educational purposes)
            long long lambda = ((3 * x * x + a) * modInverse(2 * y)) % p; // Slope of the tangent line
            long long newX = (lambda * lambda - 2 * x) % p; // x3 = lambda^2 - 2*x1
            long long newY = (lambda * (x - newX) - y) % p; // y3 = lambda * (x1 - x3) - y1
            x = newX;
            y = newY;
        }
        return make_pair(x, y);
    }

    long long modInverse(long long a) {
        a = a % p;
        for (long long x = 1; x < p; x++) {
            if ((a * x) % p == 1) return x;
        }
        return 1;
    }

    long long p = 23; // Prime modulus
};

int main() {
    // Elliptic Curve Cryptography (ECC)
    long long a = 0, b = 7; // Curve parameters for y^2 = x^3 + ax + b
    EllipticCurve ec(a, b);

    long long baseX = 2, baseY = 2; // Base point
    long long scalarAlice, scalarBob; // Private keys for Alice and Bob
    cout<<"Enter the key for Alice: ";
    cin>>scalarAlice;
    cout<<"Enter the key for Bob: ";
    cin>>scalarBob;

    // Generate public keys using scalar multiplication
    pair<long long, long long> alicePublicKey = ec.scalarMultiply(scalarAlice, baseX, baseY);
    pair<long long, long long> bobPublicKey = ec.scalarMultiply(scalarBob, baseX, baseY);

    // Perform Diffie-Hellman key exchange using shared secret generated from scalar multiplication
    pair<long long, long long> sharedSecretAlice = ec.scalarMultiply(scalarAlice, bobPublicKey.first, bobPublicKey.second);
    pair<long long, long long> sharedSecretBob = ec.scalarMultiply(scalarBob, alicePublicKey.first, alicePublicKey.second);

    // Verify if both parties have the same shared secret
    if (sharedSecretAlice == sharedSecretBob) {
        cout << "Shared secret: (" << sharedSecretAlice.first << ", " << sharedSecretAlice.second << ")" << endl;

        // Encryption and Decryption
        string plaintext = "Hello user";
        cout << "Plaintext: " << plaintext << endl;

        // Encrypt using shared secret as key
        string ciphertext;
        for (size_t i = 0; i < plaintext.length(); ++i) {
            ciphertext += plaintext[i] ^ sharedSecretAlice.first; // Simple XOR encryption
        }
        cout << "Ciphertext: " << ciphertext << endl;

        // Decrypt using shared secret as key
        string decryptedtext;
        for (size_t i = 0; i < ciphertext.length(); ++i) {
            decryptedtext += ciphertext[i] ^ sharedSecretAlice.first; // Simple XOR decryption
        }
        cout << "Decrypted Text: " << decryptedtext << endl;
    } else {
        cout << "Error: Shared secret mismatch." << endl;
    }

    return 0;
}
