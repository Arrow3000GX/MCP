# 🔧 Fix DATABASE_URL ENOTFOUND Error

## The Problem

```
"hostname": "postgres.railway.internal"
"code": "ENOTFOUND"
```

**The DATABASE_URL is using an internal hostname that doesn't work.** You need to use the PUBLIC URL instead.

---

## ⚡ Quick Fix (2 Steps)

### Step 1: Get the Correct Connection URL

In your PostgreSQL service:

1. Go to **"Connect"** or **"Database"** tab
2. Look for **"Postgres Public URL"** or **"Connection String"**
3. Copy the URL that looks like:
   ```
   postgresql://postgres:password@containers-us-west-xxx.provider.railway.app:5432/railway
   ```
   OR
   ```
   postgres://postgres:password@containers-us-west-xxx.provider.railway.app:5432/railway
   ```

### Step 2: Update DATABASE_URL Variable

In your web service:

1. Go to **"Variables"** tab
2. Find the **DATABASE_URL** variable
3. Click **"Edit"** button
4. Replace the value with the **PUBLIC URL** you just copied
5. Click **"Save"**

---

## 📋 Alternative: Use DATABASE_PUBLIC_URL

Railway provides `DATABASE_PUBLIC_URL` automatically. If it exists:

1. In your web service → Variables
2. Check if you have **`DATABASE_PUBLIC_URL`** variable
3. If yes, add/replace:
   - Name: `DATABASE_URL`
   - Value: Copy the value from `DATABASE_PUBLIC_URL`
4. Or rename the variable to `DATABASE_URL`

---

## ✅ What the Correct URL Should Look Like

**WRONG (Internal - Doesn't Work):**
```
postgresql://postgres:password@postgres.railway.internal:5432/railway
```

**CORRECT (Public - Works):**
```
postgresql://postgres:password@containers-us-west-xxx.provider.railway.app:5432/railway
```

Or:
```
postgres://postgres:password@containers-us-west-xxx.provider.railway.app:5432/railway
```

Key differences:
- ✅ `containers-us-west-xxx.provider.railway.app` (public domain)
- ❌ `postgres.railway.internal` (internal, doesn't work)

---

## 🔍 How to Find the Correct URL

### Method 1: From PostgreSQL Service Connect Tab

1. Click on your **Postgres** service
2. Go to **"Connect"** tab
3. Look for **"Postgres Connection URL"** or **"Postgres Public URL"**
4. Copy this entire URL

### Method 2: From PostgreSQL Variables

1. Go to Postgres service → **"Variables"** tab
2. Look for **`DATABASE_PUBLIC_URL`** variable
3. Copy the value (click the eye icon to reveal)

### Method 3: Check if DATABASE_PUBLIC_URL Exists

In your web service variables:
- Look for `DATABASE_PUBLIC_URL`
- If it exists, use its value
- Set it as `DATABASE_URL` (or keep both)

---

## ✅ After the Fix

Once you update the DATABASE_URL with the correct public URL:

1. **Wait** for automatic redeploy (takes ~30 seconds)
2. **Check logs** - should see:
   ```
   ✓ Database initialization successful
   ✓ Server started on port 3000
   ```
3. **Service should be accessible** at your Railway URL

---

## 🐛 Still Not Working?

If you still get errors after using the public URL:

### Check 1: Verify URL Format
Make sure the URL includes:
- Protocol: `postgresql://` or `postgres://`
- Username: `postgres:`
- Password: `(the password)@`
- Host: `containers-xxx.railway.app`
- Port: `:5432`
- Database: `/railway`

### Check 2: Check PostgreSQL Service Status
1. Go to PostgreSQL service
2. Make sure it shows **"Active"** or green status
3. If not, try restarting the service

### Check 3: Verify Connection String
Test the connection string:
- Make sure there are no extra spaces
- Make sure all special characters are URL-encoded
- Copy the entire string from Railway (don't modify it)

---

## 💡 Pro Tip

Railway automatically provides these variables for PostgreSQL:
- `DATABASE_URL` - Internal URL (doesn't always work)
- `DATABASE_PUBLIC_URL` - Public URL (always works) ✅ **Use This One!**

If you have both, **use DATABASE_PUBLIC_URL** and set it as `DATABASE_URL`.

