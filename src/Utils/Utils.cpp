#include "Utils.h"

float intrapolate(float a, float b, float value)
{
	return (value - a) / (b - a);
}

float interpolate(float a, float b, float bias)
{
	return a + ((b - a) * bias);
}

void print_partition_table() 
{
	esp_partition_iterator_t iter = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
	Serial.printf("# Name,   Type, SubType, Offset,  Size, Flags\n");
	while (iter != NULL) {
		const esp_partition_t *partition = esp_partition_get(iter);
		Serial.printf("%s, %d, %d, 0x%08x, 0x%08x,\n",
				 partition->label, partition->type, partition->subtype, partition->address, partition->size);
		iter = esp_partition_next(iter);
	}
	esp_partition_iterator_release(iter);
	Serial.printf("DONE DATA\n");

}