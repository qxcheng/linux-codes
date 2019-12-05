from django.contrib import admin
from login.models import User, ConfirmString
# Register your models here.

admin.site.register(User)
admin.site.register(ConfirmString)