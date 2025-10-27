# 🌐 How to Access Your Railway Service

## Quick Answer

Your service URL is displayed at the top of your Railway service page:
```
https://your-app-name.railway.app
```

## 📍 Where to Find the URL

### Method 1: From Service Overview
1. Go to https://railway.app
2. Click on your **"WRO"** service
3. Look at the top - you'll see:
   ```
   WRO                    [unexposed] → Click to expose
   nurturing-ambition.railway.app
   ```

### Method 2: From Deployments Tab
1. Go to **Deployments** tab
2. Click on the latest deployment
3. The URL will be shown at the top

### Method 3: Generate Public URL
If you see "unexposed":
1. Click the **"..."** button (three dots)
2. Click **"Generate Domain"** or **"Settings"** → **"Generate Domain"**
3. Copy the URL that appears

---

## 🔓 Make Service Public (If Unexposed)

Your service shows "unexposed" - you need to make it public:

### Step 1: Expose the Service
1. In your **WRO** service dashboard
2. Look for **"unexposed"** or **"..."** button
3. Click **"Generate Domain"** or **"Settings"**
4. Select **"Generate Domain"**

### Step 2: Copy the URL
After generating, you'll get a URL like:
```
https://nurturing-ambition.railway.app
```

---

## ✅ Once You Have the URL

### 1. Open in Browser
Simply paste the URL into your browser:
```
https://your-app-name.railway.app
```

### 2. Test the Health Endpoint
Check if the service is running:
```
https://your-app-name.railway.app/api/health
```

This should return:
```json
{
  "status": "ok",
  "timestamp": "..."
}
```

### 3. Access the MCP Hub Dashboard
The main interface is at:
```
https://your-app-name.railway.app
```

---

## 🐛 Troubleshooting

### "This site can't be reached"
**Problem:** Service might still be deploying or database isn't configured

**Solution:**
1. Check the **Logs** tab
2. Make sure PostgreSQL database is added
3. Verify DATABASE_URL environment variable is set
4. Wait for deployment to complete

### "503 Service Unavailable"
**Problem:** Service is still starting up

**Solution:**
1. Check **Logs** - look for startup messages
2. Wait 1-2 minutes for initial startup
3. Refresh the page

### "Connection Refused"
**Problem:** Database connection error

**Solution:**
1. Check you have PostgreSQL database service
2. Verify DATABASE_URL is set in Variables
3. See `RAILWAY-DATABASE-FIX.md` for details

---

## 📊 Check Service Status

### Via Railway Dashboard
1. Go to your service
2. Look at **Metrics** tab:
   - ✅ Green = Running
   - ⚠️ Yellow = Starting
   - ❌ Red = Error

### Via Logs
1. Go to **Logs** tab
2. Look for messages like:
   ```
   ✓ Database initialization successful
   ✓ Server started on port 3000
   ```

### Via Health Check
```bash
curl https://your-app-name.railway.app/api/health
```

Should return status 200 with health data.

---

## 🔗 Quick Links

- **Railway Dashboard:** https://railway.app
- **Service List:** https://railway.app/projects
- **Logs:** https://railway.app/your-project/logs
- **Metrics:** https://railway.app/your-project/metrics

---

## 🎯 Expected Result

Once your service is running:

1. **Navigate to:** `https://your-app-name.railway.app`
2. **You should see:** The MCP Hub login/setup page
3. **Create an account:** First-time setup
4. **Start using:** Your MCP Hub!

---

## 💡 Pro Tips

- **Bookmark the URL** - Save it for easy access
- **Check Logs regularly** - Monitor for issues
- **Use Custom Domain** - Can add your own domain in Settings
- **Enable HTTPS** - Railway does this automatically

---

## 📱 Mobile Access

Your service works on mobile too! Just use the same URL:
```
https://your-app-name.railway.app
```

The interface is responsive and works on any device.

