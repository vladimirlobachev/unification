
#include "prettyxml.h"


FILE* xml;
char* xmlfile = "tree.xml";

char* tab = "    ";
int depth = 0;

extern NTree* root;

void createHeader() {
	fprintf(xml,  "%s", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
}

void buildXMLFile() {
	xml = fopen(xmlfile, "w");

	if (!xml) {
		printf("Error: Cannot open %s file\n", xmlfile);
		exit(-1);
	}
	createHeader();
	goThrough(root);
	fclose(xml);
}

void goThrough(NTree* node) {
	int local = depth;
	bool end = false;

	char* temp = buildNode(node);
	fprintf(xml, "%s", temp);
	memset(temp, '\0', strlen(temp));
	if (node->child != NULL) {
		depth++;
		goThrough(node->child);
	}
	if (node->brother != NULL) {
		depth = local;
		end = !end;
		char* temp = malloc(sizeof(char));
		pushx(temp, 0);
		fprintf(xml, "%s", strcat(temp, "</node>\n"));
		memset(temp, '\0', strlen(temp));
		free(temp);

		goThrough(node->brother);
	}
	if (!end) {
		char* temp = malloc(sizeof(char));
		if (local) {
			depth = local;
			pushx(temp, 0);
		}
		else {
		}
		fprintf(xml, "%s", strcat(temp, "</node>\n"));
		memset(temp, '\0', strlen(temp));
		free(temp);
	}

}

char* buildNode(NTree* node) {
	char* output = malloc(MAXSIZE);
	pushx(output, 0);
	strcat(output, "<node>\n");
	if (node->data->ruletag) {
		pushx(output, 1);
		strcat(output, "<tag>");
		strcat(output, node->data->ruletag);
		strcat(output, "</tag>\n");
	}
	if (node->data->left || node->data->right) {
		pushx(output, 1);
		strcat(output, "<expr>");
		char* temp = getExpressions(node->data->left, node->data->right);
		strcat(output, temp);
		strcat(output, "</expr>\n");
		memset(temp, '\0', strlen(temp));
	}
	if (node->data->counter) {
		char* temp = getMapps(node->data->mappings);
		strcat(output, temp);
		memset(temp, '\0', strlen(temp));
	}
	return output;
}


void pushx(char* output, int degree) {
	for (int i=0; i<depth+degree; i++) {
		strcat(output, tab);
	}
}



char* getMapps(char* mappings[]) {
	char* output = malloc(MAXSIZE/4);
	for (int i=0; i<BUFFSIZE; i++) {
		if (mappings[i]) {
			pushx(output, 1);
			strcat(output, "<mapp>");
			strcat(output, mappings[i]);
			strcat(output, "</mapp>\n");
		}
		else {
			return output;
		}
	}
	return output;
}
