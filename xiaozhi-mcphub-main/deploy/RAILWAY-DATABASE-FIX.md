# 🔧 Fix Database Connection Error

## The Problem

```
Error during database initialization: {
  "code": "ECONNREFUSED"
}
```

**The app can't connect to PostgreSQL database.** You need to add a database and configure the connection.

---

## ⚡ Quick Fix (3 Steps)

### Step 1: Add PostgreSQL Database

In your Railway project:

1. Click the **"+"** button (New)
2. Select **"Database"**
3. Click **"Add PostgreSQL"**
4. Wait for it to provision (~30 seconds)

### Step 2: Get the Connection String

After the database is created:

1. Click on the **PostgreSQL** service you just created
2. Go to **"Connect"** tab
3. Copy the **"Postgres Connection URL"** - it looks like:
   ```
   postgresql://postgres:password@containers-us-west-xxx.railway.app:5432/railway
   ```

### Step 3: Add Environment Variable

1. Click on your **main web service** (the one that's failing)
2. Go to **"Variables"** tab
3. Click **"+ New Variable"**
4. Add:
   - **Variable Name:** `DATABASE_URL`
   - **Value:** (paste the connection string from Step 2)
5. Click **"Add"**

### Step 4: Redeploy

The app will automatically redeploy when you save the variable. You can also:
- Click **"Redeploy"** button
- Or trigger by pushing to GitHub

---

## 📸 Visual Guide

### Step 1: Add Database

```
Railway Dashboard
┌─────────────────────────────┐
│ [WRO Service  ]              │
│                              │
│  + New  ← Click this         │
│    ↓                        │
│  Database                    │
│    ↓                        │
│  Add PostgreSQL ← Select    │
└─────────────────────────────┘
```

### Step 2: Copy Connection String

```
PostgreSQL Service
┌─────────────────────────────┐
│ Connect                     │
│                              │
│ Postgres Connection URL:     │
│ ┌─────────────────────────┐ │
│ │ postgresql://...        │ │ ← Copy this
│ └─────────────────────────┘ │
└─────────────────────────────┘
```

### Step 3: Add to Variables

```
Main Service → Variables
┌─────────────────────────────┐
│ + New Variable              │
│                              │
│ Variable Name:              │
│ [DATABASE_URL            ]  │
│                              │
│ Value:                       │
│ [postgresql://...paste...]  │
│                              │
│ [Add]                        │
└─────────────────────────────┘
```

---

## ✅ Expected Result

After adding the DATABASE_URL variable, the deployment logs should show:

```
✓ Database initialization successful
✓ Server started successfully
✓ Listening on port 3000
```

---

## 🐛 Alternative: If Database Still Won't Connect

### Option 1: Check Database Service Status

Make sure the PostgreSQL service is running:
1. Check PostgreSQL service has green status
2. If not, click **"Redeploy"** on the PostgreSQL service

### Option 2: Verify Connection String Format

The DATABASE_URL should be:
```
postgresql://[user]:[password]@[host]:[port]/[database]
```

### Option 3: Check Network Configuration

In Railway:
1. Go to **PostgreSQL** service → **Settings**
2. Make sure **"Public Networking"** is enabled
3. Save and restart

---

## 📝 Complete Environment Variables Checklist

Your web service should have these variables:

```
DATABASE_URL=postgresql://...
NODE_ENV=production
SMART_ROUTING_ENABLED=false
ENABLE_CORS=true
```

---

## 🎯 Quick Verification

After setting up the database:

1. **Check logs** - Should see "Database initialization successful"
2. **Check URL** - Service should be accessible at `https://your-app.railway.app`
3. **Health check** - Visit `https://your-app.railway.app/api/health`

---

## 💡 Pro Tips

- **Free Tier:** Railway's free PostgreSQL has 1GB storage, perfect for testing
- **Auto-scaling:** Database scales automatically with your app
- **Backups:** Enable backups in PostgreSQL settings for production
- **Connection Pooling:** Railway handles this automatically

---

## 🚨 Still Not Working?

If you still get connection errors:

1. **Verify DATABASE_URL is set correctly**
2. **Wait 1-2 minutes** after adding database (provisioning takes time)
3. **Restart both services** (PostgreSQL and Web)
4. **Check Railway status page** for any outages

---

## ✅ Success Indicators

You'll know it's working when:
- ✅ Deployment succeeds (no errors)
- ✅ Logs show "Database initialization successful"
- ✅ App is accessible at your Railway URL
- ✅ Health check endpoint returns 200 OK

