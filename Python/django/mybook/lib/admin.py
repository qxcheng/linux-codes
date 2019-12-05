from django.contrib import admin
from lib.models import Book


class BookAdmin(admin.ModelAdmin):
    list_display = ('name', 'author', 'pub_house', 'pub_date', )
    list_filter = ['author']
    search_fields = ['name']


# Register your models here.
admin.site.register(Book, BookAdmin)
