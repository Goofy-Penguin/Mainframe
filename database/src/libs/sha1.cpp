#include <mainframe/libs/sha1.h>

namespace mainframe {
	namespace sha1 {
		// Mostly based on Paul E. Jones' sha1 implementation
		Hash sha1(const void* pMem, Basetype iLen, Hash& my_hash) {
			// if( pMem == 0 || iLen == 0 ) return my_hash;

			struct Process {
				static Basetype CircularShift(int bits, Basetype word) {
					return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32 - bits));
				}

				static void MessageBlock(Hash& H, unsigned char* Message_Block, int& Message_Block_Index) {
					const Basetype K[] = {                  // Constants defined for SHA-1
						0x5A827999,
						0x6ED9EBA1,
						0x8F1BBCDC,
						0xCA62C1D6
					};
					int     t;                          // Loop counter
					Basetype    temp;                       // Temporary word value
					Basetype    W[80];                      // Word sequence
					Basetype    A, B, C, D, E;              // Word buffers

					/*
					*  Initialize the first 16 words in the array W
					*/
					for (t = 0; t < 16; t++) {
						W[t] = ((Basetype)Message_Block[t * 4]) << 24;
						W[t] |= ((Basetype)Message_Block[t * 4 + 1]) << 16;
						W[t] |= ((Basetype)Message_Block[t * 4 + 2]) << 8;
						W[t] |= ((Basetype)Message_Block[t * 4 + 3]);
					}

					for (t = 16; t < 80; t++) {
						W[t] = CircularShift(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
					}

					A = H[0];
					B = H[1];
					C = H[2];
					D = H[3];
					E = H[4];

					for (t = 0; t < 20; t++) {
						temp = CircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0]; //D^(B&(C^D))
						temp &= 0xFFFFFFFF;
						E = D;
						D = C;
						C = CircularShift(30, B);
						B = A;
						A = temp;
					}

					for (t = 20; t < 40; t++) {
						temp = CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
						temp &= 0xFFFFFFFF;
						E = D;
						D = C;
						C = CircularShift(30, B);
						B = A;
						A = temp;
					}

					for (t = 40; t < 60; t++) {
						temp = CircularShift(5, A) +
							((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];         //(B & C) | (D & (B | C))
						temp &= 0xFFFFFFFF;
						E = D;
						D = C;
						C = CircularShift(30, B);
						B = A;
						A = temp;
					}

					for (t = 60; t < 80; t++) {
						temp = CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
						temp &= 0xFFFFFFFF;
						E = D;
						D = C;
						C = CircularShift(30, B);
						B = A;
						A = temp;
					}

					H[0] = (H[0] + A) & 0xFFFFFFFF;
					H[1] = (H[1] + B) & 0xFFFFFFFF;
					H[2] = (H[2] + C) & 0xFFFFFFFF;
					H[3] = (H[3] + D) & 0xFFFFFFFF;
					H[4] = (H[4] + E) & 0xFFFFFFFF;

					Message_Block_Index = 0;
				}
			};

			// 512-bit message blocks
			unsigned char Message_Block[64];
			// Index into message block array
			int Message_Block_Index = 0;
			// Message length in bits
			Basetype Length_Low = 0, Length_High = 0;

			if (iLen > 0) {
				// Is the message digest corrupted?
				bool Corrupted = false;

				// Input()

				unsigned char* message_array = (unsigned char*)pMem;

				while (iLen-- && !Corrupted) {
					Message_Block[Message_Block_Index++] = (*message_array & 0xFF);

					Length_Low += 8;
					Length_Low &= 0xFFFFFFFF;               // Force it to 32 bits
					if (Length_Low == 0) {
						Length_High++;
						Length_High &= 0xFFFFFFFF;          // Force it to 32 bits
						if (Length_High == 0) {
							Corrupted = true;               // Message is too long
						}
					}

					if (Message_Block_Index == 64) {
						Process::MessageBlock(my_hash, Message_Block, Message_Block_Index);
					}

					message_array++;
				}
			}

			// Result() and PadMessage()

			/*
			*  Check to see if the current message block is too small to hold
			*  the initial padding bits and length.  If so, we will pad the
			*  block, process it, and then continue padding into a second block.
			*/
			if (Message_Block_Index > 55) {
				Message_Block[Message_Block_Index++] = 0x80;

				while (Message_Block_Index < 64) {
					Message_Block[Message_Block_Index++] = 0;
				}

				Process::MessageBlock(my_hash, Message_Block, Message_Block_Index);

				while (Message_Block_Index < 56) {
					Message_Block[Message_Block_Index++] = 0;
				}
			} else {
				Message_Block[Message_Block_Index++] = 0x80;

				while (Message_Block_Index < 56) {
					Message_Block[Message_Block_Index++] = 0;
				}
			}

			/*
			*  Store the message length as the last 8 octets
			*/
			Message_Block[56] = (Length_High >> 24) & 0xFF;
			Message_Block[57] = (Length_High >> 16) & 0xFF;
			Message_Block[58] = (Length_High >> 8) & 0xFF;
			Message_Block[59] = (Length_High) & 0xFF;
			Message_Block[60] = (Length_Low >> 24) & 0xFF;
			Message_Block[61] = (Length_Low >> 16) & 0xFF;
			Message_Block[62] = (Length_Low >> 8) & 0xFF;
			Message_Block[63] = (Length_Low) & 0xFF;

			Process::MessageBlock(my_hash, Message_Block, Message_Block_Index);

			return my_hash;
		}

		Hash sha1(const void* mem, int len) {
			Hash h(5);
			h[0] = 0x67452301, h[1] = 0xEFCDAB89,
				h[2] = 0x98BADCFE, h[3] = 0x10325476,
				h[4] = 0xC3D2E1F0;
			Hash raw = sha1(mem, len, h);
			return raw;
		}
	}
}