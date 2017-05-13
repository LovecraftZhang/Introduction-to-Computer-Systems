int i;
int j;
int k;
int val;

int ping (int a, int b, int c) {
	int result;
	switch (a) {
		case 10:
		result = b + c;
		break;

		case 12:
		result = b - c;
		break;

		case 14:
		if (b > c) {
			result = 1;
		} else {
			result = 0;
		}
		break;

		case 16:
		if (c > b) {
			result = 1;
		} else {
			result = 0;
		}
		break;

		case 18:
		if (b == c) {
			result = 1;
		} else {
			result = 0;
		}
		break;

		default:
		result = 0;
		break;
	}
	return result;
}

int main (void) {
	val = ping(i, j, k);
}