#include <gtk/gtk.h>  
#include <gtk/gtkmain.h>
#include <unistd.h> 

pid_t pid;
int file;
int boolean;
int mytime;
long times;
guint timer;  

GtkWidget *window;  
GtkWidget *dialog;

GtkWidget *openFileButton;
GtkWidget *runButton;
GtkWidget *cancelButton;

GtkWidget *fileNameLabel;
GtkWidget *processLabel;
GtkWidget *threadLabel;
GtkWidget *dataLabel;
GtkWidget *patternLabel;

GtkWidget *fileNameEntry;
GtkWidget *processEntry;
GtkWidget *threadEntry;
GtkWidget *dataEntry;
GtkWidget *patternEntry;

GtkWidget *image;
GtkWidget *textView;

GtkWidget *FileSelection;

GtkWidget *hbox_one;
GtkWidget *hbox_two;
GtkWidget *hbox_three;
GtkWidget *hbox_four;
GtkWidget *hbox_five;
GtkWidget *vbox;


char string[500];
char span[1000];
char _span[10];
char _time[100];
char pattern[100];
char processName[100];
char processNumber[100];
char threadNumber[100];
char dataNumber[100];



void initData()
{
	memset(processNumber,0,100);
	memset(threadNumber,0,100);
	memset(dataNumber,0,100);
	memset(pattern,0,100);
	memset(string,0,500);
	memset(span,0,1000);
	memset(_span,0,10);
	memset(_time,0,100);
	memset(processName,0,100);
}

char *readFile()
{
	FILE *fp;  
    char *str;  
    char result[1000];  
    int filesize;  
    if ((fp=fopen("result.txt","r"))==NULL){  
        printf("打开文件失败\n");  
        return;  
    }  
    fseek(fp,0,SEEK_END);   
    filesize = ftell(fp);  
    str=(char *)malloc(filesize);  
	memset(str,0,filesize);
    str[filesize]=0;  
    rewind(fp);  
    while((fgets(result,1000,fp))!=NULL){  
        strcat(str,result);  
    }  
    fclose(fp);  
    return str;  
}


gboolean runTimer(gpointer data)   
{   
	if(boolean==0)
	{
		memset(_time,0,100);
		if(file!=1)
			strcpy(string," 计时器准备就绪");	
	}
	else
	{
		sprintf(_time,"%ld", times);  
		strcpy(string,"计算中:");
		times++;
		if(times==mytime)
		{
			strcpy(string,readFile());
			file=1;
			boolean=0;
			times=0;
			gtk_widget_hide(image);
		}
	}	
	strcpy(span,"<span foreground='black' font_desc='14'>");
	strcpy(_span,"</span>");
	strcat(span,string);
	strcat(span,_time);
	strcat(span,_span);
	gtk_label_set_markup(GTK_LABEL(textView),span);  
    return TRUE;
}  

void createProcess()
{
	char temp[500];
	memset(temp,0,500);
	strcat(temp,processName);
	strcat(temp," ");
	strcat(temp,pattern);
	strcat(temp," ");
	strcat(temp,processNumber);
	strcat(temp," ");
	strcat(temp,threadNumber);
	strcat(temp," ");
	strcat(temp,dataNumber);
	system(temp);
}


void hideWindow(GtkWidget *widget, gpointer data)
{
	gtk_widget_hide(FileSelection); 
}


void getFile(GtkWidget *widget, gpointer data)
{
	char temp[255]="程序名:";
	strcpy(processName,gtk_file_selection_get_filename (GTK_FILE_SELECTION (FileSelection)));
	strcat(temp,processName);
	gtk_entry_set_text(fileNameEntry,processName);
	gtk_widget_hide(FileSelection); 
}

void openFile(GtkWidget *widget, gpointer data)
{
	if(FileSelection!=NULL)
	{
		 gtk_widget_show (FileSelection); 
	}
	else
	{
		FileSelection = gtk_file_selection_new("程序选择");  
		gtk_file_selection_set_filename (GTK_FILE_SELECTION (FileSelection), "exec.sh");
 		g_signal_connect (GTK_OBJECT(FileSelection), "destroy",G_CALLBACK (hideWindow), NULL);
		g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(FileSelection)->ok_button),"clicked",G_CALLBACK(getFile),FileSelection);  
		g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(FileSelection)->cancel_button),"clicked",G_CALLBACK(hideWindow),FileSelection);
		gtk_file_selection_set_filename(GTK_FILE_SELECTION(FileSelection),"*");
	}
		
}

void runProgram(GtkWidget *widget, gpointer data)
{
	if(strlen(processName))
	{
		strcpy(pattern,gtk_entry_get_text(patternEntry));
		if(strlen(pattern))
		{
			strcpy(processNumber,gtk_entry_get_text(processEntry));
			if(strlen(processNumber))
			{
				strcpy(threadNumber,gtk_entry_get_text(threadEntry));
				if(strlen(threadNumber))
				{
					strcpy(dataNumber,gtk_entry_get_text(dataEntry));
					if(strlen(dataNumber))
					{
						if(strcmp(pattern,"a")==0||strcmp(pattern,"b")==0||strcmp(pattern,"c")==0)
						{
							boolean=1;
							times=0;
							file=0;
						
							mytime=rand()%8+4;

							memset(string,0,100);
							memset(span,0,300);
							memset(_span,0,10);

							strcpy(string,"计算中...................");
							strcpy(span,"<span foreground='black' font_desc='14'>");
							strcpy(_span,"</span>");
							strcat(span,string);
							strcat(span,_span);

							gtk_label_set_markup(GTK_LABEL(textView),span);  
							gtk_widget_show(image);

							if(strcmp(pattern,"a")==0)
								strcpy(pattern,"Matrix");
							if(strcmp(pattern,"b")==0)
								strcpy(pattern,"Max");
							if(strcmp(pattern,"c")==0)
								strcpy(pattern,"Sort");

							createProcess();

							}
							else
							{
								dialog = gtk_message_dialog_new (GTK_WINDOW (window),
  													GTK_DIALOG_MODAL | 
													GTK_DIALOG_DESTROY_WITH_PARENT,
  													GTK_MESSAGE_INFO,
  													GTK_BUTTONS_OK,
	  												"必须填写合法的类型{a代表Matrix,b代表Max,c代表Sort}！");
								gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),"%s\n", "\n\nProduce By SiwangHu");
								gtk_dialog_run (GTK_DIALOG (dialog));
 								gtk_widget_destroy (dialog);
							}
					}
					else
					{
						dialog = gtk_message_dialog_new (GTK_WINDOW (window),
  											GTK_DIALOG_MODAL | 
											GTK_DIALOG_DESTROY_WITH_PARENT,
  											GTK_MESSAGE_INFO,
  											GTK_BUTTONS_OK,
	  										"必须填写数据量！");
						gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),"%s\n", "\n\nProduce By SiwangHu");
						gtk_dialog_run (GTK_DIALOG (dialog));
 						gtk_widget_destroy (dialog);
					}
				}
				else
				{
					dialog = gtk_message_dialog_new (GTK_WINDOW (window),
  											GTK_DIALOG_MODAL | 
											GTK_DIALOG_DESTROY_WITH_PARENT,
  											GTK_MESSAGE_INFO,
  											GTK_BUTTONS_OK,
	  										"必须填写线程数目！");
					gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),"%s\n", "\n\nProduce By SiwangHu");
					gtk_dialog_run (GTK_DIALOG (dialog));
 					gtk_widget_destroy (dialog);
				}
				
			}
			else
			{
				dialog = gtk_message_dialog_new (GTK_WINDOW (window),
  											GTK_DIALOG_MODAL | 
											GTK_DIALOG_DESTROY_WITH_PARENT,
  											GTK_MESSAGE_INFO,
  											GTK_BUTTONS_OK,
	  										"必须填写进程数目！");
				gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),"%s\n", "\n\nProduce By SiwangHu");
				gtk_dialog_run (GTK_DIALOG (dialog));
 				gtk_widget_destroy (dialog);
			}	
		}
		else
		{
				dialog = gtk_message_dialog_new (GTK_WINDOW (window),
  											GTK_DIALOG_MODAL | 
											GTK_DIALOG_DESTROY_WITH_PARENT,
  											GTK_MESSAGE_INFO,
  											GTK_BUTTONS_OK,
	  										"必须填写类型{a代表Matrix,b代表Max,c代表Sort}！");
				gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),"%s\n", "\n\nProduce By SiwangHu");
				gtk_dialog_run (GTK_DIALOG (dialog));
 				gtk_widget_destroy (dialog);
		}
	}
	else
	{
		dialog = gtk_message_dialog_new (GTK_WINDOW (window),
  										GTK_DIALOG_MODAL | 
										GTK_DIALOG_DESTROY_WITH_PARENT,
  										GTK_MESSAGE_INFO,
  										GTK_BUTTONS_OK,
	  									"必须先选择运行的程序！");
		gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),"%s\n", "\n\nProduce By SiwangHu");
		gtk_dialog_run (GTK_DIALOG (dialog));
 		gtk_widget_destroy (dialog);
	}
}

void stopProgram(GtkWidget *widget, gpointer data)
{
	boolean=0;
	times=0;
	memset(string,0,500);
	memset(span,0,1000);
	memset(_span,0,10);
	strcpy(string," 计时器准备就绪");
	strcpy(span,"<span foreground='black' font_desc='14'>");
	strcpy(_span,"</span>");
	strcat(span,string);
	strcat(span,_span);
	gtk_label_set_markup(GTK_LABEL(textView),span);  
	gtk_widget_hide(image);
}

gint delete_event( GtkWidget *widget,GdkEvent *event,gpointer data )  
{  
	gtk_main_quit ();  
	return TRUE;  
}  
    
void destroy(GtkWidget *widget,gpointer data)  
{  
	gtk_main_quit ();  
}  


GdkPixbuf *loadImage(const gchar* filename)  
{  
    GdkPixbuf *pixbuf;  
    GError *error = NULL;  
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);  
    if(!pixbuf) {  
        fprintf(stderr, "%s\n", error->message);  
        g_error_free(error);  
    }  
    return pixbuf;  
}  


int main(int argc, char *argv[])  
{  
	initData();
	gtk_init (&argc, &argv);  
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);  
	timer = g_timeout_add(1000, (GSourceFunc)runTimer, NULL);

	FileSelection = gtk_file_selection_new("程序选择");  
	 g_signal_connect (G_OBJECT (FileSelection), "delete_event",G_CALLBACK (hideWindow), NULL);
	g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(FileSelection)->ok_button),"clicked",G_CALLBACK(getFile),FileSelection);  
	g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(FileSelection)->cancel_button),"clicked",G_CALLBACK(hideWindow),FileSelection);
	gtk_file_selection_set_filename(GTK_FILE_SELECTION(FileSelection),"*");

	gtk_window_set_title(GTK_WINDOW(window),"并行计算");  
	g_signal_connect (G_OBJECT (window), "delete_event",G_CALLBACK (delete_event), NULL);  
    g_signal_connect (G_OBJECT (window), "destroy",G_CALLBACK (destroy), NULL);   
	gtk_widget_set_size_request(window,500,380);  
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(window),5); 
	gtk_window_set_icon(GTK_WINDOW(window), loadImage("./Main.png")); 
	

	fileNameLabel=gtk_label_new("文件");	
	gtk_label_set_markup(GTK_LABEL(fileNameLabel),  "<span foreground='black' font_desc='15'>文件</span>");
	gtk_widget_show (fileNameLabel);  


	fileNameEntry=gtk_entry_new();
	gtk_editable_set_editable(fileNameEntry,FALSE);
	gtk_entry_set_max_length(fileNameEntry, 255);
	gtk_widget_show (fileNameEntry);  

	openFileButton=gtk_button_new_with_label("打开");
	g_signal_connect(G_OBJECT(openFileButton), "clicked",G_CALLBACK(openFile), "clicked");
	gtk_widget_show (openFileButton);  

	patternLabel=gtk_label_new("类型");	
	gtk_label_set_markup(GTK_LABEL(patternLabel),  "<span foreground='black' font_desc='15'>类型</span>");
	gtk_widget_show (patternLabel);  


	patternEntry=gtk_entry_new();
	gtk_entry_set_max_length(patternEntry, 255);
	gtk_widget_show (patternEntry);  
	
	hbox_one= gtk_hbox_new(FALSE,2);
	gtk_box_pack_start(hbox_one,fileNameLabel,FALSE,FALSE,5);
	gtk_box_pack_start(hbox_one,fileNameEntry,TRUE,TRUE,5);
	gtk_box_pack_start(hbox_one,openFileButton,FALSE,FALSE,5);
	gtk_box_pack_start(hbox_one,patternLabel,FALSE,FALSE,5);
	gtk_box_pack_start(hbox_one,patternEntry,FALSE,FALSE,5);



	processLabel=gtk_label_new("进程数");	
	gtk_label_set_markup(GTK_LABEL(processLabel),  "<span foreground='black' font_desc='15'>进程数</span>");
	gtk_widget_show (processLabel);  


	processEntry=gtk_entry_new();
	gtk_editable_set_editable(processEntry,TRUE);
	gtk_entry_set_max_length(processEntry, 255);
	gtk_widget_show (processEntry);  


	threadLabel=gtk_label_new("线程数");	
	gtk_label_set_markup(GTK_LABEL(threadLabel),  "<span foreground='black' font_desc='15'>线程数</span>");
	gtk_widget_show (threadLabel);  


	threadEntry=gtk_entry_new();
	gtk_editable_set_editable(threadEntry,TRUE);
	gtk_entry_set_max_length(threadEntry, 255);
	gtk_widget_show (threadEntry);  
	
	hbox_two=gtk_hbox_new(FALSE,2);
	gtk_box_pack_start(hbox_two,processLabel,FALSE,FALSE,5);
	gtk_box_pack_start(hbox_two,processEntry,TRUE,TRUE,5);
	gtk_box_pack_start(hbox_two,threadLabel,FALSE,FALSE,0);
	gtk_box_pack_start(hbox_two,threadEntry,FALSE,FALSE,0);

	dataLabel=gtk_label_new("数据量");	
	gtk_label_set_markup(GTK_LABEL(dataLabel),  "<span foreground='black' font_desc='15'>数据量</span>");
	gtk_widget_show (dataLabel);  


	dataEntry=gtk_entry_new();
	gtk_editable_set_editable(dataEntry,TRUE);
	gtk_entry_set_max_length(dataEntry, 255);
	gtk_widget_show (dataEntry);  


	runButton=gtk_button_new_with_label("运行");
	g_signal_connect(G_OBJECT(runButton), "clicked",G_CALLBACK(runProgram), "clicked");
	gtk_widget_show (runButton);  

	cancelButton=gtk_button_new_with_label("停止");
	g_signal_connect(G_OBJECT(cancelButton), "clicked",G_CALLBACK(stopProgram), "clicked");
	gtk_widget_show (cancelButton); 

	hbox_three=gtk_hbox_new(FALSE,2);
	gtk_box_pack_start(hbox_three,dataLabel,FALSE,FALSE,5);
	gtk_box_pack_start(hbox_three,dataEntry,TRUE,TRUE,5);
	gtk_box_pack_start(hbox_three,runButton,FALSE,FALSE,5);
	gtk_box_pack_start(hbox_three,cancelButton,FALSE,FALSE,5);


	hbox_four=gtk_hbox_new(FALSE,2);
	textView=gtk_label_new(" 计时器准备就绪");	
	memset(string,0,100);
	memset(span,0,300);
	memset(_span,0,10);
	strcpy(string," 计时器准备就绪");
	strcpy(span,"<span foreground='black' font_desc='15'>");
	strcpy(_span,"</span>");
	strcat(span,string);
	strcat(span,_span);
	gtk_label_set_markup(GTK_LABEL(textView), span);
	gtk_widget_show (textView);  
	gtk_box_pack_start(hbox_four,textView,TRUE,TRUE,0);

	hbox_five=gtk_hbox_new(FALSE,2);
	image = gtk_image_new_from_file("./wait.gif"); 
	gtk_widget_hide(image); 
	gtk_box_pack_start(hbox_five,image,TRUE,TRUE,0);

	vbox=gtk_vbox_new(FALSE,2);
	gtk_box_pack_start(vbox,hbox_one,FALSE,FALSE,5);
	gtk_box_pack_start(vbox,hbox_two,FALSE,FALSE,5);
	gtk_box_pack_start(vbox,hbox_three,FALSE,FALSE,20);
	gtk_box_pack_start(vbox,hbox_four,FALSE,FALSE,5);
	gtk_box_pack_start(vbox,hbox_five,FALSE,FALSE,5);

	
	
	gtk_container_add(window,vbox); 
	gtk_widget_show (hbox_one); 
	gtk_widget_show (hbox_two); 
	gtk_widget_show (hbox_three); 
	gtk_widget_show (hbox_four); 
	gtk_widget_show (hbox_five);  
	gtk_widget_show (vbox); 
    gtk_widget_show (window);  
	gtk_main ();  
          
    return 0;  
}  


















